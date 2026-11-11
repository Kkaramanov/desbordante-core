#include "core/algorithms/pp/phm/phm.h"

#include <algorithm>
#include <cstddef>
#include <optional>
#include <unordered_map>
#include <utility>
#include <vector>

#include "core/algorithms/algorithm.h"
#include "core/algorithms/pp/phm/phm_parser.h"
#include "core/algorithms/pp/phm/phui.h"
#include "core/algorithms/pp/phm/utility_list.h"
#include "core/algorithms/pp/phm/utility_transactions.h"
#include "core/config/descriptions.h"
#include "core/config/exceptions.h"
#include "core/config/names.h"
#include "core/config/option.h"
#include "core/config/option_using.h"
#include "core/util/logger.h"

namespace algos {

Phm::Phm() : Algorithm() {
    RegisterOptions();
    MakeOptionsAvailable({config::names::kPhmDataset});
}

void Phm::MakeExecuteOptsAvailable() {
    using namespace config::names;
    MakeOptionsAvailable(
            {kPhmMinUtility, kPhmMinAveragePeriodicity, kPhmMaxPeriodicity, kPhmMinPeriodicity});
}

void Phm::RegisterOptions() {
    DESBORDANTE_OPTION_USING;

    auto check_dataset_path = [](std::filesystem::path const& path) {
        if (path.empty()) {
            throw config::ConfigurationError(
                    "Option phm_dataset received an empty path. "
                    "Provide a path to a transaction dataset.");
        }
    };

    auto check_min_utility = [](phm::Utility value) {
        if (value <= 0) {
            throw config::ConfigurationError("Option phm_min_utility must be > 0; received " +
                                             std::to_string(value) +
                                             ". Set a positive utility threshold.");
        }
    };

    auto check_periodicity = [](size_t value) {
        if (value == 0) {
            throw config::ConfigurationError(
                    "Periodicity thresholds must be >= 1; received 0. "
                    "Set a positive transaction count.");
        }
    };

    auto check_average_periodicity = [](double value) {
        if (!std::isfinite(value) || value <= 0.0) {
            throw config::ConfigurationError(
                    "Average periodicity thresholds must be finite and > 0; received " +
                    std::to_string(value) + ". Set a positive finite threshold.");
        }
    };

    auto check_max_average_periodicity = [this, check_average_periodicity](double value) {
        check_average_periodicity(value);

        if (value < min_avg_periodicity_) {
            throw config::ConfigurationError(
                    "Option phm_max_average_periodicity must be >= "
                    "phm_min_average_periodicity (" +
                    std::to_string(min_avg_periodicity_) + "); received " + std::to_string(value) +
                    ". Increase the maximum or decrease the minimum.");
        }
    };

    RegisterOption(config::Option{&dataset_path_, kPhmDataset, kDPhmDataset}.SetValueCheck(
            check_dataset_path));
    RegisterOption(config::Option{&min_util_, kPhmMinUtility, kDPhmMinUtility}.SetValueCheck(
            check_min_utility));
    RegisterOption(config::Option{&min_avg_periodicity_, kPhmMinAveragePeriodicity,
                                  kDPhmMinAveragePeriodicity}
                           .SetValueCheck(check_average_periodicity)
                           .SetConditionalOpts({{{}, {kPhmMaxAveragePeriodicity}}}));
    RegisterOption(config::Option{&max_avg_periodicity_, kPhmMaxAveragePeriodicity,
                                  kDPhmMaxAveragePeriodicity}
                           .SetValueCheck(check_max_average_periodicity));
    RegisterOption(config::Option{&min_periodicity_, kPhmMinPeriodicity, kDPhmMinPeriodicity}
                           .SetValueCheck(check_periodicity));
    RegisterOption(config::Option{&max_periodicity_, kPhmMaxPeriodicity, kDPhmMaxPeriodicity}
                           .SetValueCheck(check_periodicity));
}

void Phm::LoadDataInternal() {
    std::ifstream f(dataset_path_);

    if (!f.is_open()) {
        throw std::runtime_error("Failed to open dataset: " + dataset_path_.string());
    }
    transactions_ = phm::parser::ReadDataset(f);
    database_size_ = transactions_.size();
}

void Phm::ResetState() {
    map_item_to_info_.clear();
    map_escs_.clear();
    map_eucs_.clear();
    candidate_count_ = 0;
    phuis_.clear();
}

void Phm::PrepareTransactionsForMining() {
    for (size_t i = 0; i < transactions_.size(); i++) {
        for (auto const& item : transactions_[i].items) {
            auto [it, inserted] = map_item_to_info_.try_emplace(item.item_id, ItemInfo{});
            ItemInfo& item_info = it->second;
            item_info.support++;
            item_info.twu += transactions_[i].total_utility;
            size_t periodicity = (i + 1) - item_info.last_seen_transaction;
            if (item_info.largest_periodicity < periodicity) {
                item_info.largest_periodicity = periodicity;
            }

            item_info.last_seen_transaction = i + 1;
            if (item_info.support != 1 && periodicity < item_info.smallest_periodicity) {
                item_info.smallest_periodicity = periodicity;
            }
        }
    }

    support_pruning_threshold_ = static_cast<double>(database_size_) / max_avg_periodicity_ - 1.0;

    for (auto& [item, item_info] : map_item_to_info_) {
        size_t periodicity = database_size_ - item_info.last_seen_transaction;
        if (item_info.largest_periodicity < periodicity) {
            item_info.largest_periodicity = periodicity;
        }
    }
}

[[nodiscard]] std::vector<phm::UtilityList> Phm::CreateUtilityLists() {
    std::vector<phm::UtilityList> utility_lists;
    std::unordered_map<phm::ItemId, size_t> map_item_to_utility_list;
    for (auto const& [item_id, item_info] : map_item_to_info_) {
        if (static_cast<double>(item_info.support) >= support_pruning_threshold_ &&
            item_info.largest_periodicity <= max_periodicity_ && item_info.twu >= min_util_) {
            phm::UtilityList utility_list;
            utility_list.item = item_id;
            utility_list.largest_periodicity = item_info.largest_periodicity;
            utility_list.smallest_periodicity = item_info.smallest_periodicity;
            utility_lists.push_back(std::move(utility_list));
        }
    }
    std::ranges::sort(
            utility_lists, [this](phm::ItemId a, phm::ItemId b) { return CompareItems(a, b); },
            &phm::UtilityList::item);

    for (size_t i = 0; i < utility_lists.size(); i++) {
        map_item_to_utility_list.try_emplace(utility_lists[i].item, i);
    }

    for (size_t i = 0; i < transactions_.size(); i++) {
        phm::UtilityTransaction revised_transaction;
        phm::Utility remaining_util = 0;
        for (auto const& item : transactions_[i].items) {
            ItemInfo const& item_info = map_item_to_info_.at(item.item_id);
            if (static_cast<double>(item_info.support) >= support_pruning_threshold_ &&
                item_info.largest_periodicity <= max_periodicity_ && item_info.twu >= min_util_) {
                revised_transaction.items.push_back(item);
                revised_transaction.total_utility += item.utility;
                remaining_util += item.utility;
            }
        }

        std::ranges::sort(
                revised_transaction.items,
                [this](phm::ItemId a, phm::ItemId b) { return CompareItems(a, b); },
                &phm::Item::item_id);

        for (size_t j = 0; j < revised_transaction.items.size(); j++) {
            phm::Item const& item = revised_transaction.items[j];
            remaining_util -= item.utility;
            phm::UtilityList& utility_list =
                    utility_lists[map_item_to_utility_list.at(item.item_id)];
            phm::Element element = {i + 1, item.utility, remaining_util};
            utility_list.AddElement(element);

            // Create EUCP
            auto& inner_utility_map = map_eucs_.try_emplace(item.item_id).first->second;
            for (size_t l = j + 1; l < revised_transaction.items.size(); l++) {
                inner_utility_map[revised_transaction.items[l].item_id] +=
                        revised_transaction.total_utility;
            }

            // Create ESCS
            auto& inner_containing_map = map_escs_.try_emplace(item.item_id).first->second;
            for (size_t l = j + 1; l < revised_transaction.items.size(); l++) {
                ++inner_containing_map[revised_transaction.items[l].item_id];
            }
        }
    }

    return utility_lists;
}

void Phm::MinePhuis() {
    LOG_INFO(
            "Starting Phm algorithmn: {} transactions, min_util_={}, min_avg_periodicity_={}, "
            "man_avg_periodicity_={}, min_periodicity_={}, max_periodicity={}",
            database_size_, min_util_, min_avg_periodicity_, max_avg_periodicity_, min_periodicity_,
            max_periodicity_);
    PrepareTransactionsForMining();

    auto utility_lists = CreateUtilityLists();

    std::vector<phm::ItemId> prefix;
    prefix.reserve(utility_lists.size());
    SearchPhm(prefix, nullptr, utility_lists);
}

void Phm::ExecuteInternal() {
    MinePhuis();
}

[[nodiscard]] bool Phm::CompareItems(phm::ItemId a, phm::ItemId b) const {
    auto const& info_a = map_item_to_info_.at(a);
    auto const& info_b = map_item_to_info_.at(b);

    return info_a.twu != info_b.twu ? info_a.twu < info_b.twu : a < b;
}

void Phm::SearchPhm(std::vector<phm::ItemId>& prefix, phm::UtilityList const* prefix_util_list,
                    std::vector<phm::UtilityList>& util_lists) {
    for (size_t i = 0; i < util_lists.size(); i++) {
        phm::UtilityList& x_util_list = util_lists[i];
        if (x_util_list.sum_iutils + x_util_list.sum_rutils >= min_util_) {
            prefix.push_back(x_util_list.item);
            double avg_periodicity = static_cast<double>(database_size_) /
                                     (static_cast<double>(x_util_list.GetSupport()) + 1.0);
            if (x_util_list.sum_iutils >= min_util_ && avg_periodicity <= max_avg_periodicity_ &&
                avg_periodicity >= min_avg_periodicity_ &&
                x_util_list.smallest_periodicity >= min_periodicity_ &&
                x_util_list.largest_periodicity <= max_periodicity_) {
                RegisterPhui(prefix, x_util_list, avg_periodicity);
            }

            std::vector<phm::UtilityList> ex_util_lists;

            for (size_t j = i + 1; j < util_lists.size(); j++) {
                phm::UtilityList& y_util_list = util_lists[j];

                // EUCS optimization
                if (auto const& map_iter = map_eucs_.find(x_util_list.item);
                    map_iter != map_eucs_.end()) {
                    auto const& twu_iter = map_iter->second.find(y_util_list.item);
                    if (twu_iter == map_iter->second.end() || twu_iter->second < min_util_) {
                        continue;
                    }
                }

                // ESCS optimizasion
                if (auto const& map_iter = map_escs_.find(x_util_list.item);
                    map_iter != map_escs_.end()) {
                    auto const& support_iter = map_iter->second.find(y_util_list.item);
                    if (support_iter != map_iter->second.end() &&
                        support_iter->second < support_pruning_threshold_) {
                        continue;
                    }
                }

                candidate_count_++;
                if (auto temp = Construct(prefix_util_list, x_util_list, y_util_list)) {
                    ex_util_lists.push_back(std::move(*temp));
                }
            }
            SearchPhm(prefix, &x_util_list, ex_util_lists);
            prefix.pop_back();
        }
    }
}

std::optional<phm::UtilityList> Phm::Construct(phm::UtilityList const* prefix_list,
                                               phm::UtilityList const& x_util_list,
                                               phm::UtilityList const& y_util_list) {
    phm::UtilityList pxy_util_list = phm::UtilityList{.item = y_util_list.item, .elements = {}};
    size_t last_tid = 0;
    phm::Utility total_utility = x_util_list.sum_iutils + x_util_list.sum_rutils;

    auto total_support = x_util_list.GetSupport();

    for (auto const& x_el : x_util_list.elements) {
        auto const y_el =
                std::ranges::lower_bound(y_util_list.elements, x_el.tid, {}, &phm::Element::tid);
        if (y_el == y_util_list.elements.end() || y_el->tid != x_el.tid) {
            total_utility -= x_el.iutils + x_el.rutils;
            if (total_utility < min_util_) {
                return std::nullopt;
            }

            --total_support;
            if (total_support < support_pruning_threshold_) {
                return std::nullopt;
            }
        } else if (prefix_list == nullptr) {
            size_t periodicity = x_el.tid - last_tid;
            if (periodicity > max_periodicity_) {
                return std::nullopt;
            }

            if (periodicity > pxy_util_list.largest_periodicity) {
                pxy_util_list.largest_periodicity = periodicity;
            }

            if (pxy_util_list.elements.size() > 0 &&
                periodicity < pxy_util_list.smallest_periodicity) {
                pxy_util_list.smallest_periodicity = periodicity;
            }
            last_tid = x_el.tid;

            phm::Element xy_element = {
                    .tid = x_el.tid, .iutils = x_el.iutils + y_el->iutils, .rutils = y_el->rutils};
            pxy_util_list.AddElement(xy_element);
        } else {
            auto const prefix_element = std::ranges::lower_bound(prefix_list->elements, x_el.tid,
                                                                 {}, &phm::Element::tid);
            if (prefix_element != prefix_list->elements.end() && prefix_element->tid == x_el.tid) {
                size_t periodicity = x_el.tid - last_tid;
                if (periodicity > max_periodicity_) {
                    return std::nullopt;
                }

                if (periodicity >= pxy_util_list.largest_periodicity) {
                    pxy_util_list.largest_periodicity = periodicity;
                }
                if (pxy_util_list.elements.size() > 0 &&
                    periodicity < pxy_util_list.smallest_periodicity) {
                    pxy_util_list.smallest_periodicity = periodicity;
                }
                last_tid = x_el.tid;

                phm::Element xy_element = {
                        .tid = x_el.tid,
                        .iutils = x_el.iutils + y_el->iutils - prefix_element->iutils,
                        .rutils = y_el->rutils};
                pxy_util_list.AddElement(xy_element);
            }
        }
    }

    size_t periodicity = database_size_ - last_tid;
    if (periodicity > max_periodicity_) {
        return std::nullopt;
    }
    if (periodicity > pxy_util_list.largest_periodicity) {
        pxy_util_list.largest_periodicity = periodicity;
    }
    if (static_cast<double>(pxy_util_list.GetSupport()) < support_pruning_threshold_) {
        return std::nullopt;
    }
    return pxy_util_list;
}

void Phm::RegisterPhui(std::vector<phm::ItemId> prefix, phm::UtilityList const& util_list,
                       double avg_periodicity) {
    phm::Phui result_phui(prefix, util_list.GetSupport(), util_list.smallest_periodicity,
                          util_list.largest_periodicity, avg_periodicity, util_list.sum_iutils);
    phuis_.push_back(std::move(result_phui));
}

}  // namespace algos
