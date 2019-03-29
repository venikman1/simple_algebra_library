#pragma once
#include "monomial.h"
#include "orders.h"
#include <map>

namespace SALIB {
    
    
    template <typename CoefficientType, typename Order = DefaultOrder>
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

        const CoefficientType& operator[](const Monomial &mono) const;

        bool operator==(const Polynomial& other) const;
        bool operator!=(const Polynomial& other) const;

        bool is_depends_on_variable(Monomial::VariableIndexType var_index) const; 

        Polynomial& operator+=(const Polynomial& other);
        Polynomial& operator-=(const Polynomial& other);
        Polynomial operator*(const Polynomial& other) const;
        Polynomial& operator*=(const Polynomial& other);
        Polynomial operator+(const Polynomial& other) const;
        Polynomial operator-(const Polynomial& other) const;
        Polynomial operator-() const;
        Polynomial operator+() const;

        const Monomial& get_largest_monomial() const;

        void zero();
        bool is_zero() const;

        Polynomial get_largest_monomial_as_poly() const;

        Order get_order() const;

        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;
        reverse_iterator rbegin();
        reverse_iterator rend();
        const_reverse_iterator rbegin() const;
        const_reverse_iterator rend() const;
    
    private:
        void clean_empty_monomials();

        static const CoefficientType null_coef;
        static const Monomial empty_monomial;
        MonomialMap monomials;
    };

/*
=================================IMPLEMENTATION================================= 
*/

    template <typename CoefficientType, typename Order>
    const CoefficientType Polynomial<CoefficientType, Order>::null_coef = CoefficientType(0);
    
    template <typename CoefficientType, typename Order>
    const Monomial Polynomial<CoefficientType, Order>::empty_monomial = Monomial();

    template <typename CoefficientType, typename Order>
    Order Polynomial<CoefficientType, Order>::get_order() const {
        return monomials.key_comp();
    }

    template <typename CoefficientType, typename Order>
    Polynomial<CoefficientType, Order>
    operator*(
        const Polynomial<CoefficientType, Order>& poly,
        const CoefficientType& coeff) {
        return poly * Polynomial<CoefficientType, Order>(coeff, poly.get_order());
    }

    template <typename CoefficientType, typename Order>
    Polynomial<CoefficientType, Order>
    operator*(
        const CoefficientType& coeff,
        const Polynomial<CoefficientType, Order>& poly
        ) {
        return poly * coeff;
    }

    template <typename CoefficientType, typename Order>
    Polynomial<CoefficientType, Order>
    operator+(
        const CoefficientType& coeff,
        const Polynomial<CoefficientType, Order>& poly
        ) {
        return poly + Polynomial<CoefficientType, Order>(coeff, poly.get_order());
    }

    template <typename CoefficientType, typename Order>
    Polynomial<CoefficientType, Order>::Polynomial(const Order& ord) : monomials(ord) {}

    template <typename CoefficientType, typename Order>
    Polynomial<CoefficientType, Order>::Polynomial(
        const Monomial& mono,
        const CoefficientType& coeff,
        const Order& ord
    ) : monomials(ord) {
        if (coeff != null_coef)
            monomials[mono] = coeff;
    }

    template <typename CoefficientType, typename Order>
    Polynomial<CoefficientType, Order>::Polynomial(
        const CoefficientType& coeff,
        const Order& ord
    ) : monomials(ord) {
        if (coeff != null_coef)
            monomials[empty_monomial] = coeff;
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
        Polynomial res = a * Polynomial(l / a_lt, CoefficientType(1) / a[a_lt], a.get_order());
        res -= b * Polynomial(l / b_lt, CoefficientType(1) / b[b_lt], a.get_order());
        return res;
    }

    template <typename CoefficientType, typename Order>
    template <typename CoefficientTypeOther, typename OrderOther>
    Polynomial<CoefficientType, Order>::Polynomial(
        const Polynomial<CoefficientTypeOther, OrderOther>& other,
        const Order& ord
    ) : monomials(ord) {
        for (const auto& it : other) {
            CoefficientType coeff = CoefficientType(it.second);
            if (coeff != null_coef)
                monomials[it.first] = coeff;
        }
    }

    template <typename CoefficientType, typename Order>
    const CoefficientType& Polynomial<CoefficientType, Order>::operator[](const Monomial &mono) const {
        auto found = monomials.find(mono);
        if (found == monomials.end())
            return null_coef;
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
        clean_empty_monomials();
        return *this;
    }

    template <typename CoefficientType, typename Order>
    Polynomial<CoefficientType, Order>& Polynomial<CoefficientType, Order>::operator-=(const Polynomial& other) {
        for (const auto& it : other.monomials) {
            monomials[it.first] -= it.second;
        }
        clean_empty_monomials();
        return *this;
    }

    template <typename CoefficientType, typename Order>
    Polynomial<CoefficientType, Order> Polynomial<CoefficientType, Order>::operator*(const Polynomial& other) const {
        Polynomial res(this->get_order());
        for (const auto& it1 : monomials) {
            for (const auto& it2 : other.monomials) {
                res += Polynomial(it1.first * it2.first, it1.second * it2.second);
            }
        }
        res.clean_empty_monomials();
        return res;
    }

    template <typename CoefficientType, typename Order>
    Polynomial<CoefficientType, Order>& Polynomial<CoefficientType, Order>::operator*=(const Polynomial& other) {
        Polynomial res = *this * other;
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
    bool Polynomial<CoefficientType, Order>::is_zero() const {
        return monomials.empty();
    }

    template <typename CoefficientType, typename Order>
    bool Polynomial<CoefficientType, Order>::is_depends_on_variable(Monomial::VariableIndexType var_index) const {
        for (const auto& it : monomials) {
            if (it.first.is_depends_on_variable(var_index))
                return true;
        }
        return false;
    }


    template <typename CoefficientType, typename Order>
    Polynomial<CoefficientType, Order> Polynomial<CoefficientType, Order>::operator+() const {
        Polynomial res(*this);
        return res;
    }

    template <typename CoefficientType, typename Order>
    const Monomial& Polynomial<CoefficientType, Order>::get_largest_monomial() const {
        if (is_zero())
            return empty_monomial;
        auto it = monomials.rbegin();
        return it->first;
    }

    template <typename CoefficientType, typename Order>
    Polynomial<CoefficientType, Order> Polynomial<CoefficientType, Order>::get_largest_monomial_as_poly() const {
        const Monomial& largest_mono = get_largest_monomial();
        return Polynomial(largest_mono, (*this)[largest_mono]);
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

    template <typename CoefficientType, typename Order>
    void Polynomial<CoefficientType, Order>::clean_empty_monomials() {
        for (auto it = monomials.begin(); it != monomials.end();) {
            auto it_copy = it;
            ++it;
            if (it_copy->second == null_coef)
                monomials.erase(it_copy);
        }
    }
}