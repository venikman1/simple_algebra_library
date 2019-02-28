#pragma once
#include "polynomial_set.h"
#include <vector>


namespace SALIB {
    template <typename CoefficientType, typename Order>
    class PolyAlg {
    public:
        using PolynomialType = Polynomial<CoefficientType, Order>;
        using PolySet = PolynomialSet<CoefficientType, Order>;
        static PolynomialType reduce_by(
            const PolynomialType& poly,
            const std::vector<PolynomialType>& group,
            std::vector<PolynomialType>* answer = 0
        ) {
            if (answer) answer->resize(group.size());
            PolynomialType p = poly;
            PolynomialType r;
            while (p != PolynomialType()) {
                bool divided = false;
                for (size_t i = 0; i < group.size();) {
                    Monomial p_lt = p.get_largest_monomial();
                    Monomial fi_lt = group[i].get_largest_monomial();
                    if (p_lt.is_dividable_by(fi_lt)) {
                        if (answer) (*answer)[i] += PolynomialType(p_lt / fi_lt, p[p_lt] / group[i][fi_lt], p.get_order());
                        p -= PolynomialType(p_lt / fi_lt, p[p_lt] / group[i][fi_lt], p.get_order()) * group[i];
                        divided = true;
                    }
                    else {
                        ++i;
                    }
                }
                if (!divided) {
                    r += p.get_largest_monomial_as_poly();
                    p -= p.get_largest_monomial_as_poly();
                }
            }
            return r;
        }

        template <typename PolyOrder, typename SetOrder>
        PolynomialType reduce_by(
            const Polynomial<CoefficientType, PolyOrder>& poly,
            const PolynomialSet<CoefficientType, SetOrder>& group,
            const Order& new_order = Order()
        ) {
            PolynomialType new_poly(poly, new_order);
            std::vector<PolynomialType> new_group;
            for (const auto& p : group) {
                new_group.push_back(PolynomialType(p, new_order));
            }
            return reduce_by(new_poly, new_group, nullptr);
        }
    };
}
