#pragma once
#include "monomial.h"
#include "orders.h"
#include <map>

namespace SALIB {
    
    
    template <typename CoefficientType, typename Order = MonoLexOrder>
    class Polynomial {
    public:
        using MonomialMap = std::map<Monomial, CoefficientType, Order>;
        using iterator = typename MonomialMap::iterator;
        using const_iterator = typename MonomialMap::const_iterator;
        using reverse_iterator = typename MonomialMap::reverse_iterator;
        using const_reverse_iterator = typename MonomialMap::const_reverse_iterator;

        Polynomial() = default;
        Polynomial(const Monomial& mono, const CoefficientType& coeff = CoefficientType(1)) {
            monomials[mono] = coeff;
        }
        Polynomial(const CoefficientType& coeff) {
            monomials[Monomial()] = coeff;
        }

        template <typename CoefficientTypeOther, typename OrderOther> 
        Polynomial(const Polynomial<CoefficientTypeOther, OrderOther>& other) {
            for (const auto& it : other) {
                monomials[it.first] = CoefficientType(it.second);
            }
        }

        CoefficientType& operator[](const Monomial &mono) {
            return monomials[mono];
        }

        CoefficientType operator[](const Monomial &mono) const {
            auto found = monomials.find(mono);
            if (found == monomials.end())
                return CoefficientType(0);
            return found->second;
        }

        bool operator==(const Polynomial& other) const {
            for (const auto& it : other) {
                if ((*this)[it.first] != it.second)
                    return false;
            }
            for (const auto& it : monomials) {
                if (other[it.first] != it.second)
                    return false;
            }
            return true;
        }

        bool operator!=(const Polynomial& other) const {
            return !(*this == other);
        }

        Polynomial& operator+=(const Polynomial& other) {
            for (const auto& it : other.monomials) {
                monomials[it.first] += it.second;
            }
            return *this;
        }

        Polynomial& operator-=(const Polynomial& other) {
            for (const auto& it : other.monomials) {
                monomials[it.first] -= it.second;
            }
            return *this;
        }

        Polynomial operator*(const Polynomial& other) const {
            Polynomial res;
            for (const auto& it1 : monomials) {
                for (const auto& it2 : other.monomials) {
                    res += Polynomial(it1.first * it2.first, it1.second * it2.second);
                }
            }
            return res;
        }

        Polynomial& operator*=(const Polynomial& other) {
            Polynomial res;
            for (const auto& it1 : monomials) {
                for (const auto& it2 : other.monomials) {
                    res += Polynomial(it1.first * it2.first, it1.second * it2.second);
                }
            }
            *this = res;
            return *this;
        }

        Polynomial operator+(const Polynomial& other) const {
            Polynomial res(*this);
            res += other;
            return res;
        }

        Polynomial operator-(const Polynomial& other) const {
            Polynomial res(*this);
            res -= other;
            return res;
        }

        Polynomial operator-() const {
            Polynomial res(*this);
            for (auto& it : res.monomials) {
                it.second = -it.second;
            }
            return res;
        }

        Monomial get_largest_monomial() const {
            if (monomials.size()) {
                auto it = monomials.end();
                --it;
                return it->first;
            }
            return Monomial();
        }

        iterator begin() {
            return monomials.begin();
        }

        iterator end() {
            return monomials.end();
        }

        const_iterator begin() const {
            return monomials.begin();
        }

        const_iterator end() const {
            return monomials.end();
        }

        reverse_iterator rbegin() {
            return monomials.rbegin();
        }

        reverse_iterator rend() {
            return monomials.rend();
        }

        const_reverse_iterator rbegin() const {
            return monomials.rbegin();
        }

        const_reverse_iterator rend() const {
            return monomials.rend();
        }
    
    private:
        MonomialMap monomials;
    };

    template <typename CoefficientType, typename Order>
    Polynomial<CoefficientType, Order>
    operator*(
        const Polynomial<CoefficientType, Order>& poly,
        const CoefficientType& coeff) {
        return poly * Polynomial<CoefficientType, Order>(coeff);
    }

    template <typename CoefficientType, typename Order>
    Polynomial<CoefficientType, Order>
    operator*(
        const CoefficientType& coeff,
        const Polynomial<CoefficientType, Order>& poly
        ) {
        return poly * Polynomial<CoefficientType, Order>(coeff);
    }

    // template <typename CoefficientType>
    // Polynomial<CoefficientType, MonoLexOrder>
    // operator+(const Monomial& mono1, const Monomial& mono2) {
    //     return Polynomial<CoefficientType, MonoLexOrder>(mono1) + Polynomial<CoefficientType, MonoLexOrder>(mono1);
    // }

    // template <typename CoefficientType>
    // Polynomial<CoefficientType, MonoLexOrder>
    // operator-(const Monomial& mono1, const Monomial& mono2) {
    //     return Polynomial<CoefficientType, MonoLexOrder>(mono1) + Polynomial<CoefficientType, MonoLexOrder>(mono1);
    // }

    // template <typename CoefficientType>
    // Polynomial<CoefficientType, MonoLexOrder>
    // operator-(const Monomial& mono1) {
    //     return -Polynomial<CoefficientType, MonoLexOrder>(mono1);
    // }
}