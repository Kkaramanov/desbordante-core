#include <charconv>
#include <cstddef>
#include <exception>
#include <istream>
#include <numeric>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>
#include <vector>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/constants.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>

#include "core/algorithms/pp/phm/utility_transactions.h"

namespace phm::parser {

namespace {
unsigned long ParseInt(std::string_view& sv) {
    while (!sv.empty() && (sv.front() == ' ' || sv.front() == '\t')) {
        sv.remove_prefix(1);
    }

    if (sv.empty()) {
        throw std::runtime_error("expected a number, got end of input");
    }

    unsigned long value = 0;
    auto const [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), value);

    if (ec != std::errc{}) {
        throw std::runtime_error("invalid integer");
    }

    sv.remove_prefix(ptr - sv.data());
    return value;
}

std::vector<unsigned long> ParseIntList(std::string_view sv) {
    std::vector<unsigned long> result;
    while (!sv.empty()) {
        result.push_back(ParseInt(sv));
    }

    return result;
}

std::optional<phm::UtilityTransaction> ParseTransaction(std::string const& line) {
    std::string trimed_line = line;
    boost::trim(trimed_line);

    if (trimed_line.empty() || trimed_line.front() == '#' || trimed_line.front() == '%' ||
        trimed_line.front() == '@') {
        return std::nullopt;
    }

    std::vector<std::string> parts;
    boost::split(parts, trimed_line, boost::is_any_of(":"));
    if (parts.size() != 3) {
        throw std::runtime_error("expected 3 colon-separated parts");
    }

    phm::UtilityTransaction transaction;

    std::vector<phm::ItemId> items_id = ParseIntList(parts[0]);

    boost::trim(parts[1]);
    std::string_view total_utility_sv = parts[1];
    transaction.total_utility = ParseInt(total_utility_sv);

    if (!total_utility_sv.empty()) {
        throw std::runtime_error("unexpected characters after transaction utility");
    }

    std::vector<phm::Utility> utilities = ParseIntList(parts[2]);
    if (items_id.size() != utilities.size()) {
        throw std::runtime_error("item/utility count mismatch");
    }

    if (transaction.total_utility != std::accumulate(utilities.begin(), utilities.end(), 0UL)) {
        throw std::runtime_error("transaction utility does not match item utility sum");
    }

    std::set<phm::ItemId> items_set(items_id.begin(), items_id.end());
    if (items_set.size() != items_id.size()) {
        throw std::runtime_error("duplicate item IDs");
    }

    transaction.items.reserve(items_id.size());
    for (size_t i = 0; i < items_id.size(); i++) {
        transaction.items.emplace_back(phm::Item{items_id[i], utilities[i]});
    }

    return transaction;
}
}  // namespace

[[nodiscard]] std::vector<UtilityTransaction> ReadDataset(std::istream& stream) {
    std::vector<UtilityTransaction> result;
    std::string line;
    size_t line_number = 1;
    try {
        while (std::getline(stream, line)) {
            if (auto tx = ParseTransaction(line)) {
                result.push_back(std::move(*tx));
            }
            line_number++;
        }

        if (stream.bad() || (stream.fail() && !stream.eof())) {
            throw std::runtime_error("Failed to read transaction dataset");
        }
    } catch (std::exception const& e) {
        throw std::runtime_error("Failed to parse transaction at line " +
                                 std::to_string(line_number) + ": " + e.what());
    }

    return result;
}

}  // namespace phm::parser