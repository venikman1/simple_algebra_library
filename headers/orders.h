#pragma once
#include "monomial.h"

namespace SALIB {
    
    class Order {
        int a;
        /* ... */
    };

    class MonoLexOrder {
    public:
        inline static int mono_lex_cmp(const Monomial& a, const Monomial& b);

        bool operator()(const Monomial& a, const Monomial& b) const {
            return mono_lex_cmp(a, b) < 0;
        }
    };

    
    int MonoLexOrder::mono_lex_cmp(const Monomial& a, const Monomial& b) {
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
}