#pragma once
#include <vector>
#include <unordered_set>
#include "polynomial.h"
#include <utility>

namespace SALIB {
    template <typename CoefficientType, typename Order = MonoLexOrder>
    class PolynomialHash {
    public:
        size_t operator()(const Polynomial<CoefficientType, Order>& poly) const {
            /*
                This hash might be better
            */
            size_t res = 0;
            for (const auto& mono_coef : poly) {
                size_t part = 0;
                for (const auto& num_pow : mono_coef.first) {
                    part += (num_pow.first + 1) * num_pow.second; 
                }
                res += part;
                if (part)
                    res *= base;
            }
            return res;
        }
    private:
        const static size_t base = 257;
    };

    template <typename CoefficientType, typename Order = MonoLexOrder>
    class PolynomialSet {
    public:
        using PolynomialType = Polynomial<CoefficientType, Order>;
        using HashType = PolynomialHash<CoefficientType, Order>;
        using PolynomialContainer = std::unordered_set<PolynomialType, HashType>;
        using iterator = typename PolynomialContainer::iterator;
        using const_iterator = typename PolynomialContainer::const_iterator;

        PolynomialSet() = default;

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
            PolynomialType cpy(poly);
            cpy *= PolynomialType(1 / poly[poly.get_largest_monomial()]);
            polynomials.insert(std::move(cpy));
        }

        void remove(const PolynomialType& poly) {
            PolynomialType cpy(poly);
            cpy *= PolynomialType(1 / poly[poly.get_largest_monomial()]);
            polynomials.erase(poly);
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