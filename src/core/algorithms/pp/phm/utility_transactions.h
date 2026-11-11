#pragma once

#include <vector>

namespace phm {

using Utility = unsigned long;
using ItemId = unsigned long;

struct Item {
    ItemId item_id;
    Utility utility;
};

struct UtilityTransaction {
    std::vector<Item> items;
    Utility total_utility = 0;
};

}  // namespace phm