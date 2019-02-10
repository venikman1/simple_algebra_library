#pragma once
#include <vector>
#include "polynomial.h"
#include <iostream>

namespace SALIB {

    template <typename CoefficientType, typename Order = MonoLexOrder>
    class PolynomialSet {
    public:
        using PolynomialType = Polynomial<CoefficientType, Order>;
        using PolynomialContainer = std::vector<PolynomialType>;
        using iterator = typename PolynomialContainer::iterator;
        using const_iterator = typename PolynomialContainer::const_iterator;
        using reverse_iterator = typename PolynomialContainer::reverse_iterator;
        using const_reverse_iterator = typename PolynomialContainer::const_reverse_iterator;

        PolynomialSet() = default;

        PolynomialType reduce(const PolynomialType& poly, PolynomialSet& answer) const {
            answer.polynomials.assign(size(), PolynomialType());
            PolynomialType p = poly;
            PolynomialType r;
            while (p != PolynomialType()) {
                bool divided = false;
                for (size_t i = 0; i < size();) {
                    Monomial p_lt = p.get_largest_monomial();
                    Monomial fi_lt = polynomials[i].get_largest_monomial();
                    if (p_lt.is_dividable_by(fi_lt)) {
                        answer[i] += PolynomialType(p_lt / fi_lt, p[p_lt] / polynomials[i][fi_lt]);
                        p -= PolynomialType(p_lt / fi_lt, p[p_lt] / polynomials[i][fi_lt]) * polynomials[i];
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

        PolynomialSet get_groebner_basis() const {
            PolynomialSet basis = *this;
            PolynomialSet answer;
            while (true) {
                std::vector<PolynomialType> to_add;
                for (int i = 0; i < basis.size(); ++i) {
                    for (int j = i + 1; j < basis.size(); ++j) {
                        PolynomialType s = PolynomialType::s_polynomial(basis[i], basis[j]);
                        s = basis.reduce(s, answer);
                        if (s != PolynomialType())
                            to_add.push_back(s);
                    }
                }
                if (to_add.size()) {
                    for (const auto& p : to_add) {
                        basis.add(p);
                    }
                }
                else {
                    break;
                }
            }
            return basis;
        }

        void add(const PolynomialType& poly) {
            polynomials.push_back(poly);
        }

        void clear() {
            polynomials.clear();
        }

        PolynomialType& operator[](size_t id) {
            return polynomials[id];
        }

        const PolynomialType& operator[](size_t id) const {
            return polynomials[id];
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

        reverse_iterator rbegin() {
            return polynomials.rbegin();
        }

        reverse_iterator rend() {
            return polynomials.rend();
        }

        const_reverse_iterator rbegin() const {
            return polynomials.rbegin();
        }

        const_reverse_iterator rend() const {
            return polynomials.rend();
        }

    private:
        PolynomialContainer polynomials;
    };
}