#pragma once

#include <cstddef>
#include <filesystem>
#include <limits>
#include <optional>
#include <unordered_map>
#include <vector>

#include "core/algorithms/algorithm.h"
#include "core/algorithms/pp/phm/phui.h"
#include "core/algorithms/pp/phm/utility_list.h"
#include "core/algorithms/pp/phm/utility_transactions.h"

namespace algos {

class Phm : public Algorithm {
private:
    using EucsMap = std::unordered_map<phm::ItemId, std::unordered_map<phm::ItemId, phm::Utility>>;
    using EscsMap = std::unordered_map<phm::ItemId, std::unordered_map<phm::ItemId, size_t>>;

    struct ItemInfo {
        phm::Utility twu = 0;
        size_t last_seen_transaction = 0;
        int support = 0;
        size_t largest_periodicity = 0;
        size_t smallest_periodicity = std::numeric_limits<size_t>::max();
    };

    std::filesystem::path dataset_path_;

    std::vector<phm::UtilityTransaction> transactions_;

    std::vector<phm::Phui> phuis_;

    std::unordered_map<phm::ItemId, ItemInfo> map_item_to_info_;

    EucsMap map_eucs_;

    EscsMap map_escs_;

    phm::Utility min_util_;

    double min_avg_periodicity_;

    double max_avg_periodicity_;

    size_t min_periodicity_;

    size_t max_periodicity_;

    double support_pruning_threshold_;

    size_t database_size_ = 0;
    size_t candidate_count_ = 0;

    void LoadDataInternal() override;
    void ExecuteInternal() override;
    void ResetState() override;
    void RegisterOptions();

    void MinePhuis();
    void PrepareTransactionsForMining();
    [[nodiscard]] std::vector<phm::UtilityList> CreateUtilityLists();

    [[nodiscard]] bool CompareItems(phm::ItemId a, phm::ItemId b) const;

    void RegisterPhui(std::vector<phm::ItemId> prefix, phm::UtilityList const& util_list,
                      double avg_periodicity);

    void SearchPhm(std::vector<phm::ItemId>& prefix, phm::UtilityList const* prefix_util_list,
                   std::vector<phm::UtilityList>& util_lists);

    std::optional<phm::UtilityList> Construct(phm::UtilityList const* prefix_list,
                                              phm::UtilityList const& x_util_list,
                                              phm::UtilityList const& y_util_list);

protected:
    void MakeExecuteOptsAvailable() override;

public:
    [[nodiscard]] std::vector<phm::Phui> GetPhuis() const noexcept {
        return phuis_;
    };

    Phm();
};

}  // namespace algos