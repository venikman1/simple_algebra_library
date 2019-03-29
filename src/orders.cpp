#include "orders.h"

namespace SALIB {
    int MonoLexOrder::cmp(const Monomial& a, const Monomial& b) {
        auto a_it = a.begin();
        auto b_it = b.begin();
        while (a_it != a.end() && b_it != b.end()) {
            if (*a_it < *b_it)
                return -1;
            if (*a_it > *b_it)
                return 1;
            ++a_it;
            ++b_it;
        }
        if (a_it == a.end()) {
            while (b_it != b.end()) {
                if (*b_it)
                    return -1;
                ++b_it;
            }
            return 0;
        }
        if (b_it == b.end()) {
            while (a_it != a.end()) {
                if (*a_it)
                    return 1;
                ++a_it;
            }
            return 0;
        }
        return 0;
    }

    bool MonoLexOrder::operator()(const Monomial& a, const Monomial& b) const {
        return cmp(a, b) < 0;
    }

    int GradientSemiOrder::cmp(const Monomial& a, const Monomial& b) {
        long long res_a = 0, res_b = 0;
        for (const auto& pw : a) {
            res_a += pw;
        }
        for (const auto& pw : b) {
            res_b += pw;
        }
        return res_a - res_b;
    }

    bool GradientSemiOrder::operator()(const Monomial& a, const Monomial& b) const {
        return cmp(a, b) < 0;
    }


    CustomOrder::CustomOrder() {
        compare = [](const Monomial& a, const Monomial& b) -> int {return 0;};
    }

    bool CustomOrder::operator()(const Monomial& a, const Monomial& b) const {
        return compare(a, b) < 0;
    }

    int CustomOrder::cmp(const Monomial& a, const Monomial& b) const {
        return compare(a, b);
    }
};
