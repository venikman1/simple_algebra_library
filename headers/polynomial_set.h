#pragma once
#include <vector>
#include <unordered_set>
#include "polynomial.h"
#include "orders.h"
#include <utility>
#include <iostream>
#include <boost/rational.hpp>

namespace boost {
    using namespace SALIB;
    inline size_t hash_value(const Monomial& mono);

    template <typename IntType>
    size_t hash_value(const boost::rational<IntType>& coeff);

    template <typename CoefficientType, typename Order>
    size_t hash_value(const Polynomial<CoefficientType, Order>& poly);
}

#include <boost/functional/hash.hpp>

namespace SALIB {
    template <typename CoefficientType, typename Order = MonoLexOrder>
    class PolynomialSet {
    public:
        using PolynomialType = Polynomial<CoefficientType, Order>;
        using HashType = boost::hash<PolynomialType>;
        using PolynomialContainer = std::unordered_set<PolynomialType, HashType>;
        using const_iterator = typename PolynomialContainer::const_iterator;

        PolynomialSet() = default;

        void add(const PolynomialType& poly);
        void remove(const PolynomialType& poly);

        PolynomialSet& operator+=(const PolynomialSet& other);
        PolynomialSet operator+(const PolynomialSet& other) const;
        PolynomialSet& operator*=(const PolynomialType& poly);
        PolynomialSet operator*(const PolynomialType& poly) const;

        void remove_depending_polynomials(Monomial::VariableIndexType var_index);

        void clear();

        size_t size() const;

        const_iterator begin() const;

        const_iterator end() const;

    private:
        PolynomialContainer polynomials;
    };

/*
=================================IMPLEMENTATION================================= 
*/
    
    template <typename CoefficientType, typename Order>
    void PolynomialSet<CoefficientType, Order>::add(const PolynomialType& poly) {
        if (poly.is_zero())
            return;
        PolynomialType cpy(poly);
        cpy *= PolynomialType(CoefficientType(1) / poly[poly.get_largest_monomial()]);
        polynomials.insert(cpy);
    }

    template <typename CoefficientType, typename Order>
    void PolynomialSet<CoefficientType, Order>::remove(const PolynomialType& poly) {
        if (poly.is_zero())
            return;
        PolynomialType cpy(poly);
        cpy *= PolynomialType(CoefficientType(1) / poly[poly.get_largest_monomial()]);
        polynomials.erase(cpy);
    }

    template <typename CoefficientType, typename Order>
    PolynomialSet<CoefficientType, Order>&
    PolynomialSet<CoefficientType, Order>::operator+=(const PolynomialSet& other) {
        for (const auto& p : other) {
            add(p);
        }
        return *this;
    }

    template <typename CoefficientType, typename Order>
    PolynomialSet<CoefficientType, Order> 
    PolynomialSet<CoefficientType, Order>::operator+(const PolynomialSet& other) const {
        PolynomialSet res(*this);
        res += other;
        return res;
    }

    template <typename CoefficientType, typename Order>
    PolynomialSet<CoefficientType, Order>&
    PolynomialSet<CoefficientType, Order>::operator*=(const PolynomialType& poly) {
        (*this) = (*this) * poly;
        return *this;
    }

    template <typename CoefficientType, typename Order>
    PolynomialSet<CoefficientType, Order>
    PolynomialSet<CoefficientType, Order>::operator*(const PolynomialType& poly) const {
        PolynomialSet res;
        for (auto it = polynomials.begin(); it != polynomials.end(); ++it) {
            res.add(poly * (*it));
        }
        return res;
    }

    template <typename CoefficientType, typename Order>
    void 
    PolynomialSet<CoefficientType, Order>::remove_depending_polynomials(
        Monomial::VariableIndexType var_index
    ) {
        for (auto it = polynomials.begin(); it != polynomials.end();) {
            auto it_copy = it;
            ++it;
            if (it_copy->is_depends_on_variable(var_index))
                polynomials.erase(it_copy);
        }
    }

    template <typename CoefficientType, typename Order>
    void PolynomialSet<CoefficientType, Order>::clear() {
        polynomials.clear();
    }

    template <typename CoefficientType, typename Order>
    size_t PolynomialSet<CoefficientType, Order>::size() const {
        return polynomials.size();
    }

    template <typename CoefficientType, typename Order>
    typename PolynomialSet<CoefficientType, Order>::const_iterator 
    PolynomialSet<CoefficientType, Order>::begin() const {
        return polynomials.begin();
    }

    template <typename CoefficientType, typename Order>
    typename PolynomialSet<CoefficientType, Order>::const_iterator
    PolynomialSet<CoefficientType, Order>::end() const {
        return polynomials.end();
    }
}

namespace boost {
    using namespace SALIB;
    size_t hash_value(const Monomial& mono)
    {
        size_t seed = 0;
        size_t idx = 0;
        for (auto deg : mono) {
            if (deg) {
                boost::hash_combine(seed, deg);
                boost::hash_combine(seed, idx);
            }
            ++idx;
        }
        return seed;
    }

    template <typename IntType>
    size_t hash_value(const boost::rational<IntType>& coeff)
    {
        size_t seed = 0;
        boost::hash_combine(seed, coeff.numerator());
        boost::hash_combine(seed, coeff.denominator());
        return seed;
    }

    template <typename CoefficientType, typename Order>
    size_t hash_value(const Polynomial<CoefficientType, Order>& poly) {
        boost::hash<Monomial> mono_hasher;
        boost::hash<CoefficientType> coeff_hasher;

        size_t seed = 0;
        for (const auto& c_m : poly) {
            boost::hash_combine(seed, mono_hasher(c_m.first));
            boost::hash_combine(seed, coeff_hasher(c_m.second));
        }
        return seed;
    }

}
