#pragma once
#include "polynomial_set.h"
#include <vector>
#include <queue>
#include <iostream>
#include <utility>

namespace SALIB {
    template <typename CoefficientType, typename Order = DefaultOrder>
    class PolyAlg {
    public:
        using PolynomialType = Polynomial<CoefficientType, Order>;
        using PolySet = PolynomialSet<CoefficientType, Order>;

        static PolynomialType reduce_by(
            PolynomialType divider,
            const std::vector<PolynomialType>& divisors,
            std::vector<PolynomialType>* incomplete_quotients = 0
        );

        template <typename PolyOrder, typename SetOrder>
        static PolynomialType reduce_by(
            const Polynomial<CoefficientType, PolyOrder>& divider,
            const PolynomialSet<CoefficientType, SetOrder>& divisors
        );

        static void make_groebner_basis(std::vector<PolynomialType>& ideal);

        static PolySet auto_reduce(const PolySet& ideal);

        template <typename SetOrder>
        static PolySet make_groebner_basis(
            const PolynomialSet<CoefficientType, SetOrder>& ideal
        );

        template <typename PolyOrder, typename SetOrder>
        static bool is_polynomial_in_ideal(
            const Polynomial<CoefficientType, PolyOrder>& poly,
            const PolynomialSet<CoefficientType, SetOrder>& ideal
        );

        template <typename PolyOrder, typename SetOrder>
        static bool is_polynomial_in_radical(
            const Polynomial<CoefficientType, PolyOrder>& poly,
            const PolynomialSet<CoefficientType, SetOrder>& ideal,
            Monomial::VariableIndexType free_variable
        );

        template <typename SetOrder>
        static PolySet intersect_ideals(
            const PolynomialSet<CoefficientType, SetOrder>& ideal1,
            const PolynomialSet<CoefficientType, SetOrder>& ideal2,
            Monomial::VariableIndexType free_variable
        );
    private:
        static bool try_to_reduce(
                PolynomialType& divider,
                const std::vector<PolynomialType>& divisors,
                std::vector<PolynomialType>* incomplete_quotients);

        static bool reduce_by_one(
                PolynomialType& divider,
                const PolynomialType& divisor,
                PolynomialType* incomplete_quotient);
    };

    class PairMaker {
    public:
        inline explicit PairMaker(size_t n);

        class iterator {
        public:
            using Value = std::pair<size_t, size_t>;

            inline iterator(Value v);

            inline iterator& operator++();
            inline iterator operator++(int);
            inline const Value& operator*();
            inline const Value* operator->();

            inline bool operator==(const iterator& other);
            inline bool operator!=(const iterator& other);
        private:
            Value value;
        };

        inline iterator begin() const;
        inline iterator end() const;
    private:
        size_t n;
    };


    /*
=================================IMPLEMENTATION=================================
*/
    PairMaker::iterator::iterator(PairMaker::iterator::Value v) : value(std::move(v)) {}

    PairMaker::iterator &PairMaker::iterator::operator++() {
        if (value.first + 1 == value.second) {
            ++value.second;
            value.first = 0;
        }
        else
            ++value.first;
        return *this;
    }

    PairMaker::iterator PairMaker::iterator::operator++(int) {
        iterator copy(*this);
        ++(*this);
        return copy;
    }

    const PairMaker::iterator::Value &PairMaker::iterator::operator*() {
        return value;
    }

    const PairMaker::iterator::Value *PairMaker::iterator::operator->() {
        return &value;
    }

    bool PairMaker::iterator::operator==(const PairMaker::iterator &other) {
        return value == other.value;
    }

    bool PairMaker::iterator::operator!=(const PairMaker::iterator &other) {
        return value != other.value;
    }

    PairMaker::iterator PairMaker::begin() const {
        if (n < 2)
            return PairMaker::iterator({-1, -1});
        return PairMaker::iterator({0, 1});
    }

    PairMaker::iterator PairMaker::end() const {
        if (n < 2)
            return PairMaker::iterator({-1, -1});
        return PairMaker::iterator({n - 2, n});
    }

    PairMaker::PairMaker(size_t n) : n(n) {}

    template <typename CoefficientType, typename Order>
    bool PolyAlg<CoefficientType, Order>::reduce_by_one(
            PolynomialType& divider,
            const PolynomialType& divisor,
            PolynomialType* incomplete_quotient) {
        Monomial divider_lt = divider.get_largest_monomial();
        Monomial divisor_lt = divisor.get_largest_monomial();
        if (divider.is_zero() || !divider_lt.is_dividable_by(divisor_lt))
            return false;

        while (!divider.is_zero() && divider_lt.is_dividable_by(divisor_lt)) {
            PolynomialType subs(divider[divider_lt] / divisor[divisor_lt], divider_lt / divisor_lt);
            if (incomplete_quotient)
                (*incomplete_quotient) += subs;
            divider -= subs * divisor;

            divider_lt = divider.get_largest_monomial();
            divisor_lt = divisor.get_largest_monomial();
        }
        return true;
    }

    template <typename CoefficientType, typename Order>
    bool PolyAlg<CoefficientType, Order>::try_to_reduce(
            PolynomialType& divider,
            const std::vector<PolynomialType>& divisors,
            std::vector<PolynomialType>* incomplete_quotients) {
        bool is_reduced = false;
        for (size_t i = 0; i < divisors.size() && not divider.is_zero(); ++i) {
            is_reduced |= reduce_by_one(divider, divisors[i], (incomplete_quotients) ? &(*incomplete_quotients)[i] : nullptr);
        }
        return is_reduced;
    }

    template <typename CoefficientType, typename Order>
    typename PolyAlg<CoefficientType, Order>::PolynomialType
    PolyAlg<CoefficientType, Order>::reduce_by(
        PolynomialType divider,
        const std::vector<PolynomialType>& divisors,
        std::vector<PolynomialType>* incomplete_quotients
    ) {
        if (incomplete_quotients) {
            incomplete_quotients->assign(divisors.size(), PolynomialType());
        }
        PolynomialType rest;
        while (!divider.is_zero()) {
            while (try_to_reduce(divider, divisors, incomplete_quotients)) {}

            rest += divider.get_largest_monomial_as_poly();
            divider -= divider.get_largest_monomial_as_poly();

        }
        return rest;
    }

    template <typename CoefficientType, typename Order>
    template <typename PolyOrder, typename SetOrder>
    typename PolyAlg<CoefficientType, Order>::PolynomialType 
    PolyAlg<CoefficientType, Order>::reduce_by(
        const Polynomial<CoefficientType, PolyOrder>& divider,
        const PolynomialSet<CoefficientType, SetOrder>& divisors
    ) {
        PolynomialType new_divider(divider);
        std::vector<PolynomialType> new_divisors;
        new_divisors.reserve(divisors.size());
        for (const auto& p : divisors) {
            new_divisors.push_back(PolynomialType(p));
        }
        return reduce_by(new_divider, new_divisors, nullptr);
    }

    template <typename CoefficientType, typename Order>
    typename PolyAlg<CoefficientType, Order>::PolySet PolyAlg<CoefficientType, Order>::auto_reduce(const PolySet& ideal) {
        PolySet res(ideal);
        for (const auto& poly : ideal) {
            res.remove(poly);
            PolynomialType reduced = reduce_by(poly, res);
            res.add(reduced);
        }
        return res;
    }

    template <typename CoefficientType, typename Order>
    void
    PolyAlg<CoefficientType, Order>::make_groebner_basis(
        std::vector<PolynomialType>& ideal
    ) {
        std::queue<std::pair<size_t, size_t>> pairs;
        for (const auto& pr : PairMaker(ideal.size()))
            pairs.push(pr);

        while (!pairs.empty()) {
            size_t i = pairs.front().first;
            size_t j = pairs.front().second;
            if (i + 1 == j)
                std::cerr << j << " processed, ideal size is " << ideal.size() << "\n";
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
                PolynomialType s = PolynomialType::s_polynomial(ideal[i], ideal[j]);
                s = reduce_by(s, ideal);
                if (!s.is_zero()) {
//                    std::cerr << "Ideal size is " << ideal.size() << "\n";
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
        const PolynomialSet<CoefficientType, SetOrder>& ideal
    ) {
        std::vector<PolynomialType> new_ideal;
        new_ideal.reserve(ideal.size());
        for (const auto& p : ideal) {
            new_ideal.push_back(PolynomialType(p));
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
        const PolynomialSet<CoefficientType, SetOrder>& ideal
        )
    {
        PolySet basis = make_groebner_basis(ideal);
        PolynomialType reduced = reduce_by(poly, basis);
        return reduced.is_zero();
    }

    template <typename CoefficientType, typename Order>
    template <typename PolyOrder, typename SetOrder>
    bool 
    PolyAlg<CoefficientType, Order>::is_polynomial_in_radical(
        const Polynomial<CoefficientType, PolyOrder>& poly,
        const PolynomialSet<CoefficientType, SetOrder>& ideal,
        Monomial::VariableIndexType free_variable
        )
    {
        PolynomialType t = PolynomialType(CoefficientType(1), Monomial(free_variable, 1));
        PolynomialType one = PolynomialType(CoefficientType(1));
        PolySet ideal_copy(ideal);
        ideal_copy.add(one - t * poly);
        PolySet basis = make_groebner_basis(ideal_copy);
        PolynomialType reduced = reduce_by(one, basis);
        return reduced.is_zero();
        
    }

    template <typename CoefficientType, typename Order>
    template <typename SetOrder>
    typename PolyAlg<CoefficientType, Order>::PolySet
    PolyAlg<CoefficientType, Order>::intersect_ideals(
        const PolynomialSet<CoefficientType, SetOrder>& ideal1,
        const PolynomialSet<CoefficientType, SetOrder>& ideal2,
        Monomial::VariableIndexType free_variable
        )
    {
        PolynomialType t = PolynomialType(CoefficientType(1), Monomial(free_variable, 1));
        PolynomialType one = PolynomialType(CoefficientType(1));
        PolySet new_ideal = ideal1 * t + ideal2 * (one - t);
        PolySet basis = make_groebner_basis(new_ideal);
        basis.remove_depending_polynomials(free_variable);
        return basis;
    }
}
