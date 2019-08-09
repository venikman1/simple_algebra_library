#pragma once
#include "polynomial.h"
#include <iostream>
#include <string>

namespace SALIB {
    template <typename CoefficientType, typename Order>
    std::ostream& operator<<(std::ostream& out, const Polynomial<CoefficientType, Order>& poly);



/*
=================================IMPLEMENTATION================================= 
*/

//    template <typename CoefficientType>
//    void print_monomial(std::ostream& out, const Monomial& mono, const CoefficientType* coeff) {
//        out <<
//    }

    template <typename CoefficientType, typename Order>
    std::ostream& operator<<(std::ostream& out, const Polynomial<CoefficientType, Order>& poly) {
        if (poly.is_zero()) {
            out << "0";
            return out;
        }
        bool first_time = true;
        for (auto it = poly.rbegin(); it != poly.rend(); ++it) {     
            if (it->second != CoefficientType(0)) {
                if (first_time)
                    first_time = false;      
                else
                    out << " + ";
                out << it->second;
                int i = 0;
                for (const auto& var : it->first) {
                    if (var) {
                        out << " * ";
                        if (i < 3)
                            out << (char)('x' + i);
                        else
                            out << "x_" << i;
                        out << "^" << var;
                    }
                    ++i;
                }
            }
        }
        return out;
    }
}
