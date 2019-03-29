#pragma once
#include <vector>
#include <unordered_set>
#include "polynomial.h"
#include "orders.h"
#include <utility>
#include <iostream>

namespace std {
    template <>
    struct hash<SALIB::Monomial>
    {
        inline size_t operator()(const SALIB::Monomial& mono) const;

        const static size_t base = 227;
    };

    template <typename T>
    struct hash<boost::rational<T>>
    {
        inline size_t operator()(const boost::rational<long long>& num) const;

        const static size_t a = 15485941;
        const static size_t b = 15487399;
    };
    
    template <typename CoefficientType, typename Order>
    struct hash<SALIB::Polynomial<CoefficientType, Order>>
    {
        inline size_t operator()(const SALIB::Polynomial<CoefficientType, Order>& poly) const;
        const static size_t base = 283;
    };
}

namespace SALIB {
    inline size_t overflowing_binary_pow(size_t x, size_t y);

    template <typename CoefficientType, typename Order = MonoLexOrder>
    class PolynomialSet {
    public:
        using PolynomialType = Polynomial<CoefficientType, Order>;
        using HashType = std::hash<PolynomialType>;
        using PolynomialContainer = std::unordered_set<PolynomialType, HashType>;
        using iterator = typename PolynomialContainer::iterator;
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
}
/*
=================================IMPLEMENTATION================================= 
*/

namespace std {
    size_t hash<SALIB::Monomial>::operator()(const SALIB::Monomial& mono) const {
        size_t res = 0;
        for (auto it = mono.rbegin(); it != mono.rend(); ++it) {
            res *= base;
            res += *it;
        }
        return res;
    }

    template <typename T>
    size_t hash<boost::rational<T>>::operator()(const boost::rational<long long>& num) const {
        return ((size_t)num.numerator() * a) + ((size_t)num.denominator() * b);
    }
    
    template <typename CoefficientType, typename Order>
    size_t hash<SALIB::Polynomial<CoefficientType, Order>>::operator()(
        const SALIB::Polynomial<CoefficientType, Order>& poly
    ) const {
        const static std::hash<SALIB::Monomial> mono_hash;
        const static std::hash<CoefficientType> coeff_hash; 
        size_t res = 0;
        for (const auto& mono_coef : poly) {
            res += SALIB::overflowing_binary_pow(
                mono_hash(mono_coef.first),
                coeff_hash(mono_coef.second)
            );
        }
        return res;
    }
}

namespace SALIB {
    size_t overflowing_binary_pow(size_t x, size_t y) {
        if (y == 0)
            return 1;
        if (y & 1) {
            return overflowing_binary_pow(x, y ^ 1) * y;
        } else {
            size_t p = overflowing_binary_pow(x, y >> 1);
            return p * p;
        }
    }
    
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
};
