#pragma once

#include <algorithm>
#include <cstddef>
#include <limits>
#include <locale>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <boost/container_hash/hash.hpp>

#include "core/algorithms/pp/phm/utility_transactions.h"

namespace phm {

struct Phui {
    // Items are stored in ascending order of ItemID.
    std::vector<ItemId> items;

    Utility utility;

    size_t support;

    size_t min_periodicity;

    size_t max_periodicity;

    double avg_periodicity;

    Phui(std::vector<ItemId> items, size_t support, size_t min_periodicity, size_t max_periodicity,
         double avg_periodicity, Utility utility)
        : items(std::move(items)),
          utility(utility),
          support(support),
          min_periodicity(min_periodicity),
          max_periodicity(max_periodicity),
          avg_periodicity(avg_periodicity) {
        std::sort(this->items.begin(), this->items.end());
    }

    struct Hash {
        size_t operator()(Phui const& phui) const {
            size_t seed = 0;
            boost::hash_combine(seed, phui.support);
            boost::hash_combine(seed, phui.utility);
            boost::hash_combine(seed, phui.min_periodicity);
            boost::hash_combine(seed, phui.max_periodicity);
            boost::hash_combine(seed, phui.avg_periodicity);
            for (auto it : phui.items) {
                boost::hash_combine(seed, it);
            }

            return seed;
        }
    };

    std::string ToString() const {
        std::ostringstream output;
        output.imbue(std::locale::classic());

        output << '{';
        for (size_t i = 0; i < items.size(); ++i) {
            if (i != 0) {
                output << ", ";
            }
            output << items[i];
        }

        output << "} #UTIL: " << utility << " #SUP: " << support << " #MINPER: ";

        if (min_periodicity == std::numeric_limits<size_t>::max()) {
            output << "inf";
        } else {
            output << min_periodicity;
        }

        output << " #MAXPER: " << max_periodicity << " #AVGPER: " << avg_periodicity;

        return output.str();
    }

    bool operator==(Phui const& other) const = default;
};
}  // namespace phm
