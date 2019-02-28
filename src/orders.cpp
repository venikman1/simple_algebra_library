#include "orders.h"

namespace SALIB {
    int MonoLexOrder::cmp(const Monomial& a, const Monomial& b) {
        auto it1 = a.begin();
        auto it2 = b.begin();
        while (it1 != a.end() || it2 != b.end()) {
            while (it1->second == 0 && it1 != a.end())
                ++it1;
            while (it2->second == 0 && it2 != b.end())
                ++it2;
            if (it2 == b.end() && it1 == a.end())
                return 0;
            if (it1 == a.end())
                return -1;
            if (it2 == b.end())
                return 1;
            if (it1->first < it2->first)
                return 1;
            if (it1->first > it2->first)
                return -1;
            if (it1->second < it2->second)
                return -1;
            if (it1->second > it2->second)
                return 1;
            ++it1;
            ++it2;
        }
        return 0;
    }

    bool MonoLexOrder::operator()(const Monomial& a, const Monomial& b) const {
        return cmp(a, b) < 0;
    }
};
