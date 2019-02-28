#pragma once
#include "polynomial.h"
#include <iostream>
#include <string>

namespace SALIB {
    template <typename CoefficientType, typename Order>
    std::ostream& operator<<(std::ostream& out, const Polynomial<CoefficientType, Order>& poly) {
        if (poly == Polynomial<CoefficientType, Order>()) {
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
                for (const auto& var : it->first) {
                    if (var.second) {
                        out << " * ";
                        if (var.first < 3 && var.first >= 0)
                            out << (char)('x' + var.first);
                        else
                            out << "x_" << var.first;
                        out << "^" << var.second;
                    }
                }
            }
        }
        return out;
    }
}
