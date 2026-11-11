#pragma once

#include <cstddef>
#include <limits>
#include <vector>

#include "core/algorithms/pp/phm/utility_transactions.h"

namespace phm {

struct Element {
    size_t tid;
    Utility iutils;
    Utility rutils;
};

struct UtilityList {
    ItemId item;
    Utility sum_iutils = 0;
    Utility sum_rutils = 0;

    std::vector<Element> elements;

    size_t largest_periodicity = 0;
    size_t smallest_periodicity = std::numeric_limits<size_t>::max();

    size_t GetSupport() const {
        return elements.size();
    }

    void AddElement(Element const& el) {
        sum_iutils += el.iutils;
        sum_rutils += el.rutils;
        elements.push_back(el);
    }
};

}  // namespace phm