#pragma once
#include <vector>
#include <unordered_set>
#include "polynomial.h"
#include "orders.h"
#include <utility>

namespace SALIB {
    inline size_t overflowing_binary_pow(size_t x, size_t y) {
        if (y == 0)
            return 1;
        if (y & 1) {
            return overflowing_binary_pow(x, y ^ 1) * y;
        } else {
            size_t p = overflowing_binary_pow(x, y >> 1);
            return p * p;
        }
    }
}

namespace std {
    template <>
    struct hash<SALIB::Monomial>
    {
        size_t operator()(const SALIB::Monomial& mono) const {
            size_t res = 0;
            for (auto it = mono.rbegin(); it != mono.rend(); ++it) {
                res *= base;
                res += *it;
            }
            return res;
        }

        const static size_t base = 227;
    };

    template <typename T>
    struct hash<boost::rational<T>>
    {
        size_t operator()(const boost::rational<long long>& num) const {
            return ((size_t)num.numerator() * a) + ((size_t)num.denominator() * b);
        }

        const static size_t a = 15485941;
        const static size_t b = 15487399;
    };
    
    template <typename CoefficientType, typename Order>
    struct hash<SALIB::Polynomial<CoefficientType, Order>>
    {
        size_t operator()(const SALIB::Polynomial<CoefficientType, Order>& poly) const {
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
    const static size_t base = 283;
    };
}

namespace SALIB {
    template <typename CoefficientType, typename Order = MonoLexOrder>
    class PolynomialSet {
    public:
        using PolynomialType = Polynomial<CoefficientType, Order>;
        using HashType = std::hash<PolynomialType>;
        using PolynomialContainer = std::unordered_set<PolynomialType, HashType>;
        using iterator = typename PolynomialContainer::iterator;
        using const_iterator = typename PolynomialContainer::const_iterator;

        PolynomialSet() = default;

        void add(const PolynomialType& poly) {
            if (poly.is_zero())
                return;
            PolynomialType cpy(poly);
            cpy *= PolynomialType(CoefficientType(1) / poly[poly.get_largest_monomial()]);
            polynomials.insert(cpy);
        }

        void remove(const PolynomialType& poly) {
            if (poly.is_zero())
                return;
            PolynomialType cpy(poly);
            cpy *= PolynomialType(CoefficientType(1) / poly[poly.get_largest_monomial()]);
            polynomials.erase(cpy);
        }

        PolynomialSet& operator+=(const PolynomialSet& other) {
            for (const auto& p : other) {
                add(p);
            }
            return *this;
        }

        PolynomialSet operator+(const PolynomialSet& other) const {
            PolynomialSet res(*this);
            res += other;
            return res;
        }

        // PolynomialSet operator*=(const PolynomialType& poly) {
        //     for (auto it = polynomials.begin(); it != polynomials.end(); ++it) {
        //         // (*it) *= poly;
        //     }
        //     return *this;
        // }

        PolynomialSet operator*(const PolynomialType& poly) const {
            PolynomialSet res;
            for (auto it = polynomials.begin(); it != polynomials.end(); ++it) {
                res.add(poly * (*it));
            }
            return res;
        }

        void remove_depending_polynomials(Monomial::VariableIndexType var_index) {
            for (auto it = polynomials.begin(); it != polynomials.end();) {
                auto it_copy = it;
                ++it;
                if (it_copy->is_depends_on_variable(var_index))
                    polynomials.erase(it_copy);
            }
        }

        void clear() {
            polynomials.clear();
        }

        size_t size() const {
            return polynomials.size();
        }

        iterator begin() {
            return polynomials.begin();
        }

        iterator end() {
            return polynomials.end();
        }

        const_iterator begin() const {
            return polynomials.begin();
        }

        const_iterator end() const {
            return polynomials.end();
        }

    private:
        PolynomialContainer polynomials;
    };
}