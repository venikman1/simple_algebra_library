#pragma once
#include "polynomial_set.h"
#include <vector>
#include <queue>
#include <iostream>

namespace SALIB {
    template <typename CoefficientType, typename Order = DefaultOrder>
    class PolyAlg {
    public:
        using PolynomialType = Polynomial<CoefficientType, Order>;
        using PolySet = PolynomialSet<CoefficientType, Order>;

        static PolynomialType reduce_by(
            const PolynomialType& divider,
            const std::vector<PolynomialType>& divisors,
            std::vector<PolynomialType>* incomplete_quotients = 0
        );

        template <typename PolyOrder, typename SetOrder>
        static PolynomialType reduce_by(
            const Polynomial<CoefficientType, PolyOrder>& divider,
            const PolynomialSet<CoefficientType, SetOrder>& divisors,
            const Order& new_order = Order()
        );

        static void make_groebner_basis(std::vector<PolynomialType>& ideal);

        template <typename SetOrder>
        static PolySet make_groebner_basis(
            const PolynomialSet<CoefficientType, SetOrder>& ideal,
            const Order& new_order = Order()
        );

        template <typename PolyOrder, typename SetOrder>
        static bool is_polynomial_in_ideal(
            const Polynomial<CoefficientType, PolyOrder>& poly,
            const PolynomialSet<CoefficientType, SetOrder>& ideal,
            const Order& new_order = Order()
        );

        template <typename PolyOrder, typename SetOrder>
        static bool is_polynomial_in_radical(
            const Polynomial<CoefficientType, PolyOrder>& poly,
            const PolynomialSet<CoefficientType, SetOrder>& ideal,
            Monomial::VariableIndexType free_variable,
            const Order& new_order = Order()
        );

        template <typename SetOrder>
        static PolySet intersect_ideals(
            const PolynomialSet<CoefficientType, SetOrder>& ideal1,
            const PolynomialSet<CoefficientType, SetOrder>& ideal2,
            Monomial::VariableIndexType free_variable,
            const Order& new_order = Order()
        );
    };

/*
=================================IMPLEMENTATION================================= 
*/

    template <typename CoefficientType, typename Order>
    typename PolyAlg<CoefficientType, Order>::PolynomialType
    PolyAlg<CoefficientType, Order>::reduce_by(
        const PolynomialType& divider,
        const std::vector<PolynomialType>& divisors,
        std::vector<PolynomialType>* incomplete_quotients
    ) {
        if (incomplete_quotients) incomplete_quotients->resize(divisors.size());
        PolynomialType p = divider;
        PolynomialType r(p.get_order());
        while (!p.is_zero()) {
            bool divided = false;
            for (size_t i = 0; i < divisors.size();) {
                Monomial p_lt = p.get_largest_monomial();
                Monomial fi_lt = divisors[i].get_largest_monomial();
                if (p_lt.is_dividable_by(fi_lt)) {
                    if (incomplete_quotients) 
                        (*incomplete_quotients)[i] += PolynomialType(p_lt / fi_lt, p[p_lt] / divisors[i][fi_lt], p.get_order());
                    p -= PolynomialType(p_lt / fi_lt, p[p_lt] / divisors[i][fi_lt], p.get_order()) * divisors[i];
                    divided = true;
                    if (p.is_zero())
                        break;
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

    template <typename CoefficientType, typename Order>
    template <typename PolyOrder, typename SetOrder>
    typename PolyAlg<CoefficientType, Order>::PolynomialType 
    PolyAlg<CoefficientType, Order>::reduce_by(
        const Polynomial<CoefficientType, PolyOrder>& divider,
        const PolynomialSet<CoefficientType, SetOrder>& divisors,
        const Order& new_order
    ) {
        PolynomialType new_divider(divider, new_order);
        std::vector<PolynomialType> new_divisors;
        new_divisors.reserve(divisors.size());
        for (const auto& p : divisors) {
            new_divisors.push_back(PolynomialType(p, new_order));
        }
        return reduce_by(new_divider, new_divisors, nullptr);
    }

    template <typename CoefficientType, typename Order>
    void
    PolyAlg<CoefficientType, Order>::make_groebner_basis(
        std::vector<PolynomialType>& ideal
    ) {
        std::queue<std::pair<size_t, size_t>> pairs;
        for (size_t i = 0; i < ideal.size(); ++i) {
            for (size_t j = 0; j < i; ++j) {
                pairs.push(std::make_pair(j, i));
            }
        }
        while (!pairs.empty()) {
            size_t i = pairs.front().first;
            size_t j = pairs.front().second;
            pairs.pop();
            Monomial i_lt = ideal[i].get_largest_monomial();
            Monomial j_lt = ideal[j].get_largest_monomial();
            Monomial lcm_i_j = Monomial::lcm(i_lt, j_lt);

            bool criterion = false;
            for (size_t l = 0; l < i; ++l) {
                if (lcm_i_j.is_dividable_by(ideal[l].get_largest_monomial()))
                    criterion = true;
            }
            if (lcm_i_j != i_lt * j_lt && !criterion) {
                PolynomialType s = PolynomialType::s_polynomial(ideal[i], ideal[j], ideal[i].get_order());
                s = reduce_by(s, ideal);
                if (!s.is_zero()) {
                    ideal.push_back(s);
                    for (size_t t = 0; t < ideal.size() - 1; ++t) {
                        pairs.push(std::make_pair(t, ideal.size() - 1));
                    }
                }
            }
        }
    }

    template <typename CoefficientType, typename Order>
    template <typename SetOrder>
    typename PolyAlg<CoefficientType, Order>::PolySet
    PolyAlg<CoefficientType, Order>::make_groebner_basis(
        const PolynomialSet<CoefficientType, SetOrder>& ideal,
        const Order& new_order
    ) {
        std::vector<PolynomialType> new_ideal;
        new_ideal.reserve(ideal.size());
        for (const auto& p : ideal) {
            new_ideal.push_back(PolynomialType(p, new_order));
        }
        make_groebner_basis(new_ideal);
        PolySet basis;
        for (const auto& p : new_ideal) {
            basis.add(p);
        }
        return basis;
    }

    template <typename CoefficientType, typename Order>
    template <typename PolyOrder, typename SetOrder>
    bool
    PolyAlg<CoefficientType, Order>::is_polynomial_in_ideal(
        const Polynomial<CoefficientType, PolyOrder>& poly,
        const PolynomialSet<CoefficientType, SetOrder>& ideal,
        const Order& new_order)
    {
        PolySet basis = make_groebner_basis(ideal, new_order);
        PolynomialType reduced = reduce_by(poly, basis, new_order);
        return reduced.is_zero();
    }

    template <typename CoefficientType, typename Order>
    template <typename PolyOrder, typename SetOrder>
    bool 
    PolyAlg<CoefficientType, Order>::is_polynomial_in_radical(
        const Polynomial<CoefficientType, PolyOrder>& poly,
        const PolynomialSet<CoefficientType, SetOrder>& ideal,
        Monomial::VariableIndexType free_variable,
        const Order& new_order)
    {
        PolynomialType t = PolynomialType(Monomial(free_variable, 1), CoefficientType(1), new_order);
        PolynomialType one = PolynomialType(CoefficientType(1), new_order);
        PolySet ideal_copy(ideal);
        ideal_copy.add(one - t * poly);
        PolySet basis = make_groebner_basis(ideal_copy, new_order);
        PolynomialType reduced = reduce_by(one, basis, new_order);
        return reduced.is_zero();
        
    }

    template <typename CoefficientType, typename Order>
    template <typename SetOrder>
    typename PolyAlg<CoefficientType, Order>::PolySet
    PolyAlg<CoefficientType, Order>::intersect_ideals(
        const PolynomialSet<CoefficientType, SetOrder>& ideal1,
        const PolynomialSet<CoefficientType, SetOrder>& ideal2,
        Monomial::VariableIndexType free_variable,
        const Order& new_order)
    {
        PolynomialType t = PolynomialType(Monomial(free_variable, 1), CoefficientType(1), new_order);
        PolynomialType one = PolynomialType(CoefficientType(1), new_order);
        PolySet new_ideal = ideal1 * t + ideal2 * (one - t);
        PolySet basis = make_groebner_basis(new_ideal, new_order);
        basis.remove_depending_polynomials(free_variable);
        return basis;
    }
}
