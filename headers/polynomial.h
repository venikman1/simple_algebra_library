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

        Polynomial(const Order& ord = Order());
        Polynomial(const Monomial& mono, const CoefficientType& coeff = CoefficientType(1), const Order& ord = Order());
        Polynomial(const CoefficientType& coeff, const Order& ord = Order());

        static Polynomial s_polynomial(const Polynomial& a, const Polynomial& b, const Order& ord = Order());

        template <typename CoefficientTypeOther, typename OrderOther> 
        Polynomial(const Polynomial<CoefficientTypeOther, OrderOther>& other, const Order& ord = Order());

        CoefficientType& operator[](const Monomial &mono);

        CoefficientType operator[](const Monomial &mono) const;

        bool operator==(const Polynomial& other) const;
        bool operator!=(const Polynomial& other) const;

        Polynomial& operator+=(const Polynomial& other);
        Polynomial& operator-=(const Polynomial& other);
        Polynomial operator*(const Polynomial& other) const;
        Polynomial& operator*=(const Polynomial& other);
        Polynomial operator+(const Polynomial& other) const;
        Polynomial operator-(const Polynomial& other) const;
        Polynomial operator-() const;
        Polynomial operator+() const;

        Monomial get_largest_monomial() const;

        void zero();

        Polynomial get_largest_monomial_as_poly() const;

        const Order& get_order() const;

        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;
        reverse_iterator rbegin();
        reverse_iterator rend();
        const_reverse_iterator rbegin() const;
        const_reverse_iterator rend() const;
    
    private:
        MonomialMap monomials;
        Order order;
    };

    template <typename CoefficientType, typename Order>
    const Order& Polynomial<CoefficientType, Order>::get_order() const {
        return order;
    }

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

    template <typename CoefficientType, typename Order>
    Polynomial<CoefficientType, Order>
    operator+(
        const CoefficientType& coeff,
        const Polynomial<CoefficientType, Order>& poly
        ) {
        return poly + Polynomial<CoefficientType, Order>(coeff);
    }

    template <typename CoefficientType, typename Order>
    Polynomial<CoefficientType, Order>::Polynomial(const Order& ord) : monomials(ord) {}

    template <typename CoefficientType, typename Order>
    Polynomial<CoefficientType, Order>::Polynomial(
        const Monomial& mono,
        const CoefficientType& coeff,
        const Order& ord
    ) : monomials(ord), order(ord) {
        monomials[mono] = coeff;
    }

    template <typename CoefficientType, typename Order>
    Polynomial<CoefficientType, Order>::Polynomial(
        const CoefficientType& coeff,
        const Order& ord
    ) : monomials(ord), order(ord) {
        monomials[Monomial()] = coeff;
    }

    template <typename CoefficientType, typename Order>
    Polynomial<CoefficientType, Order> Polynomial<CoefficientType, Order>::s_polynomial(
        const Polynomial& a,
        const Polynomial& b,
        const Order& ord
    ) {
        Monomial a_lt = a.get_largest_monomial();
        Monomial b_lt = b.get_largest_monomial();
        Monomial l = Monomial::lcm(a_lt, b_lt);
        Polynomial res = a * Polynomial(l / a_lt, CoefficientType(1) / a[a_lt], ord);
        res -= b * Polynomial(l / b_lt, CoefficientType(1) / b[b_lt]);
        return res;
    }

    template <typename CoefficientType, typename Order>
    template <typename CoefficientTypeOther, typename OrderOther>
    Polynomial<CoefficientType, Order>::Polynomial(
        const Polynomial<CoefficientTypeOther,OrderOther>& other,
        const Order& ord
    ) : monomials(ord) {
        for (const auto& it : other) {
            monomials[it.first] = CoefficientType(it.second);
        }
    }

    template <typename CoefficientType, typename Order>
    CoefficientType& Polynomial<CoefficientType, Order>::operator[](const Monomial &mono) {
        return monomials[mono];
    }

    template <typename CoefficientType, typename Order>
    CoefficientType Polynomial<CoefficientType, Order>::operator[](const Monomial &mono) const {
        auto found = monomials.find(mono);
        if (found == monomials.end())
            return CoefficientType(0);
        return found->second;
    }

    template <typename CoefficientType, typename Order>
    bool Polynomial<CoefficientType, Order>::operator==(const Polynomial& other) const {
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

    template <typename CoefficientType, typename Order>
    bool Polynomial<CoefficientType, Order>::operator!=(const Polynomial& other) const {
        return !(*this == other);
    }

    template <typename CoefficientType, typename Order>
    Polynomial<CoefficientType, Order>& Polynomial<CoefficientType, Order>::operator+=(const Polynomial& other) {
        for (const auto& it : other.monomials) {
            monomials[it.first] += it.second;
        }
        return *this;
    }

    template <typename CoefficientType, typename Order>
    Polynomial<CoefficientType, Order>& Polynomial<CoefficientType, Order>::operator-=(const Polynomial& other) {
        for (const auto& it : other.monomials) {
            monomials[it.first] -= it.second;
        }
        return *this;
    }

    template <typename CoefficientType, typename Order>
    Polynomial<CoefficientType, Order> Polynomial<CoefficientType, Order>::operator*(const Polynomial& other) const {
        Polynomial res;
        for (const auto& it1 : monomials) {
            for (const auto& it2 : other.monomials) {
                res += Polynomial(it1.first * it2.first, it1.second * it2.second);
            }
        }
        return res;
    }

    template <typename CoefficientType, typename Order>
    Polynomial<CoefficientType, Order>& Polynomial<CoefficientType, Order>::operator*=(const Polynomial& other) {
        Polynomial res;
        for (const auto& it1 : monomials) {
            for (const auto& it2 : other.monomials) {
                res += Polynomial(it1.first * it2.first, it1.second * it2.second);
            }
        }
        *this = res;
        return *this;
    }

    template <typename CoefficientType, typename Order>
    Polynomial<CoefficientType, Order> Polynomial<CoefficientType, Order>::operator+(const Polynomial& other) const {
        Polynomial res(*this);
        res += other;
        return res;
    }

    template <typename CoefficientType, typename Order>
    Polynomial<CoefficientType, Order> Polynomial<CoefficientType, Order>::operator-(const Polynomial& other) const {
        Polynomial res(*this);
        res -= other;
        return res;
    }

    template <typename CoefficientType, typename Order>
    Polynomial<CoefficientType, Order> Polynomial<CoefficientType, Order>::operator-() const {
        Polynomial res(*this);
        for (auto& it : res.monomials) {
            it.second = -it.second;
        }
        return res;
    }

    template <typename CoefficientType, typename Order>
    void Polynomial<CoefficientType, Order>::zero() {
        monomials.clear();
    }

    template <typename CoefficientType, typename Order>
    Polynomial<CoefficientType, Order> Polynomial<CoefficientType, Order>::operator+() const {
        Polynomial res(*this);
        return res;
    }

    template <typename CoefficientType, typename Order>
    Monomial Polynomial<CoefficientType, Order>::get_largest_monomial() const {
        for (auto it = monomials.rbegin(); it != monomials.rend(); ++it) {
            if (it->second != CoefficientType(0))
                return it->first;
        }
        return Monomial();
    }

    template <typename CoefficientType, typename Order>
    Polynomial<CoefficientType, Order> Polynomial<CoefficientType, Order>::get_largest_monomial_as_poly() const {
        for (auto it = monomials.rbegin(); it != monomials.rend(); ++it) {
            if (it->second != CoefficientType(0))
                return Polynomial(it->first, it->second);
        }
        return Polynomial();
    }

    template <typename CoefficientType, typename Order>
    typename Polynomial<CoefficientType, Order>::iterator Polynomial<CoefficientType, Order>::begin() {
        return monomials.begin();
    }

    template <typename CoefficientType, typename Order>
    typename Polynomial<CoefficientType, Order>::iterator Polynomial<CoefficientType, Order>::end() {
        return monomials.end();
    }

    template <typename CoefficientType, typename Order>
    typename Polynomial<CoefficientType, Order>::const_iterator Polynomial<CoefficientType, Order>::begin() const {
        return monomials.begin();
    }

    template <typename CoefficientType, typename Order>
    typename Polynomial<CoefficientType, Order>::const_iterator Polynomial<CoefficientType, Order>::end() const {
        return monomials.end();
    }

    template <typename CoefficientType, typename Order>
    typename Polynomial<CoefficientType, Order>::reverse_iterator Polynomial<CoefficientType, Order>::rbegin() {
        return monomials.rbegin();
    }

    template <typename CoefficientType, typename Order>
    typename Polynomial<CoefficientType, Order>::reverse_iterator Polynomial<CoefficientType, Order>::rend() {
        return monomials.rend();
    }

    template <typename CoefficientType, typename Order>
    typename Polynomial<CoefficientType, Order>::const_reverse_iterator Polynomial<CoefficientType, Order>::rbegin() const {
        return monomials.rbegin();
    }

    template <typename CoefficientType, typename Order>
    typename Polynomial<CoefficientType, Order>::const_reverse_iterator Polynomial<CoefficientType, Order>::rend() const {
        return monomials.rend();
    }
}