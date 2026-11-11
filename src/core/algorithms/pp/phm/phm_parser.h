#pragma once

#include <istream>
#include <vector>

#include "core/algorithms/pp/phm/utility_transactions.h"

namespace phm::parser {

[[nodiscard]] std::vector<UtilityTransaction> ReadDataset(std::istream& stream);

}  // namespace phm::parser