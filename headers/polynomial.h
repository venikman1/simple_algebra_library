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

        Polynomial& operator+=(const Polynomial& other) {
            for (auto& it : other.monomials) {
                monomials[it.first] += it.second;
            }
            return *this;
        }

        Polynomial& operator-=(const Polynomial& other) {
            for (auto& it : other.monomials) {
                monomials[it.first] -= it.second;
            }
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
}