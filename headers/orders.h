#pragma once
#include "monomial.h"

namespace SALIB {
    
    class Order {
        int a;
        /* ... */
    };

    class MonoLexOrder {
    public:
        bool operator()(const Monomial& a, const Monomial& b) const {
            auto it1 = a.begin();
            auto it2 = b.begin();
            while (it1 != a.end() || it2 != b.end()) {
                while (it1->second == 0 && it1 != a.end())
                    ++it1;
                while (it2->second == 0 && it2 != b.end())
                    ++it2;
                if (it2 == b.end())
                    return false;
                if (it1 == a.end())
                    return true;
                if (it1->first < it2->first)
                    return false;
                if (it1->first > it2->first)
                    return true;
                if (it1->second < it2->second)
                    return true;
                if (it1->second > it2->second)
                    return false;
                ++it1;
                ++it2;
            }
            return false;
        }
    };
}