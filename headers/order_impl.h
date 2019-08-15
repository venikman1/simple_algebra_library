#pragma once

#include "monomial.h"
#include "orders.h"
/*
=================================IMPLEMENTATION=================================
*/
namespace SALIB {
    template<typename Order>
    int RevOrder<Order>::cmp(const Monomial &a, const Monomial &b) {
        return -Order::cmp(a, b);
    }

    template<typename Order>
    bool RevOrder<Order>::operator()(const Monomial &a, const Monomial &b) const {
        return RevOrder<Order>::cmp(a, b) < 0;
    }


    template<typename FirstOrder, typename ... Orders>
    int CustomOrder<FirstOrder, Orders ...>::cmp(const Monomial &a, const Monomial &b) {
        int res = FirstOrder::cmp(a, b);
        if (res == 0)
            return CustomOrder<Orders ...>::cmp(a, b);
        return res;
    }

    template<typename FirstOrder, typename ... Orders>
    bool CustomOrder<FirstOrder, Orders ...>::operator()(const Monomial &a, const Monomial &b) const {
        return CustomOrder<FirstOrder, Orders ...>::cmp(a, b) < 0;
    }

    template<typename FirstOrder>
    int CustomOrder<FirstOrder>::cmp(const Monomial &a, const Monomial &b) {
        return FirstOrder::cmp(a, b);
    }


    int MonoLexOrder::cmp(const Monomial &a, const Monomial &b) {
        auto a_it = a.begin();
        auto b_it = b.begin();
        while (a_it != a.end() && b_it != b.end()) {
            if (*a_it < *b_it)
                return -1;
            if (*a_it > *b_it)
                return 1;
            ++a_it;
            ++b_it;
        }
        if (a_it == a.end()) {
            while (b_it != b.end()) {
                if (*b_it)
                    return -1;
                ++b_it;
            }
            return 0;
        }
        if (b_it == b.end()) {
            while (a_it != a.end()) {
                if (*a_it)
                    return 1;
                ++a_it;
            }
            return 0;
        }
        return 0;
    }

    bool MonoLexOrder::operator()(const Monomial &a, const Monomial &b) const {
        return cmp(a, b) < 0;
    }

    int MonoGradientSemiOrder::cmp(const Monomial &a, const Monomial &b) {
        return a.get_degree() - b.get_degree();
    }

    bool MonoGradientSemiOrder::operator()(const Monomial &a, const Monomial &b) const {
        return cmp(a, b) < 0;
    }
}