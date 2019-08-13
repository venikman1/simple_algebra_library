#pragma once
#include "polynomial.h"
#include <iostream>
#include <string>

namespace SALIB {
    template <typename CoefficientType, typename Order>
    std::ostream& operator<<(std::ostream& out, const Polynomial<CoefficientType, Order>& poly);
    inline std::ostream& operator<<(std::ostream& out, const Monomial& mono);
    inline void print_variable(std::ostream& out, Monomial::VariableIndexType var, Monomial::VariableDegreeType deg);

    const std::string ALPHABET = "abcdefghijklmnopqrstuvwxyz";
/*
=================================IMPLEMENTATION================================= 
*/

    void print_variable(std::ostream& out, Monomial::VariableIndexType var, Monomial::VariableDegreeType deg) {
        if (var < ALPHABET.size())
            out << ALPHABET[var];
        else
            out << "x_" << var;
        out << "^" << deg;
    }

    std::ostream& operator<<(std::ostream& out, const Monomial& mono) {
        bool first_var = true;
        for (auto it = mono.begin(); it != mono.end(); ++it) {
            int idx = it - mono.begin();
            if (*it) {
                if (!first_var) {
                    out << " * ";
                } else {
                    first_var = false;
                }
                print_variable(out, idx, *it);
            }
        }
        return out;
    }

    template <typename CoefficientType, typename Order>
    std::ostream& operator<<(std::ostream& out, const Polynomial<CoefficientType, Order>& poly) {
        if (poly.is_zero()) {
            return out << "0";
        }
        for (auto it = poly.rbegin(); it != poly.rend(); ++it) {
            if (it != poly.rbegin())
                out << " + ";
            out << it->second << (it->first.is_zero() ? "" : " * ") << it->first;
        }
        return out;
    }
}
