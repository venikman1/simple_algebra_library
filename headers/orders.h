#pragma once

#include <functional>
#include <utility>

#include "monomial.h"


namespace SALIB {

    class MonoLexOrder {
    public:
        static int cmp(const Monomial& a, const Monomial& b);

        bool operator()(const Monomial& a, const Monomial& b) const;
    };

    class MonoGradientSemiOrder {
    public:
        static int cmp(const Monomial& a, const Monomial& b);

        bool operator()(const Monomial& a, const Monomial& b) const;
    };

    template <typename Order>
    class RevOrder {
    public:
        static int cmp(const Monomial& a, const Monomial& b);

        bool operator()(const Monomial& a, const Monomial& b) const;
    };

    template <typename FirstOrder, typename ... Orders>
    class CustomOrder {
    public:
        bool operator()(const Monomial& a, const Monomial& b) const;

        static int cmp(const Monomial& a, const Monomial& b);

    private:
        using Derived = CustomOrder<Orders ...>;
    };

    template <typename FirstOrder>
    class CustomOrder<FirstOrder> {
    public:
        static int cmp(const Monomial& a, const Monomial& b);
    };

    using DefaultOrder = MonoLexOrder;

    /*
    =================================IMPLEMENTATION=================================
    */

    template <typename Order>
    int RevOrder<Order>::cmp(const Monomial& a, const Monomial& b) {
        return -Order::cmp(a, b);
    }

    template <typename Order>
    bool RevOrder<Order>::operator()(const Monomial& a, const Monomial& b) const {
        return RevOrder<Order>::cmp(a, b) < 0;
    }


    template <typename FirstOrder, typename ... Orders>
    int CustomOrder<FirstOrder, Orders ...>::cmp(const Monomial& a, const Monomial& b) {
        int res = FirstOrder::cmp(a, b);
        if (res == 0)
            return Derived::cmp(a, b);
        return res;
    }

    template <typename FirstOrder, typename ... Orders>
    bool CustomOrder<FirstOrder, Orders ...>::operator()(const Monomial& a, const Monomial& b) const {
        return CustomOrder<FirstOrder, Orders ...>::cmp(a, b) < 0;
    }

    template<typename FirstOrder>
    int CustomOrder<FirstOrder>::cmp(const Monomial& a, const Monomial& b) {
        return FirstOrder::cmp(a, b);
    }
}