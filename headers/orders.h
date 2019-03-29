#pragma once
#include "monomial.h"
#include <functional>
#include <utility>

namespace SALIB {
    class MonoLexOrder {
    public:
        static int cmp(const Monomial& a, const Monomial& b);

        bool operator()(const Monomial& a, const Monomial& b) const;
    };

    class GradientSemiOrder {
    public:
        static int cmp(const Monomial& a, const Monomial& b);

        bool operator()(const Monomial& a, const Monomial& b) const;
    };

    class CustomOrder {
    public:
        CustomOrder();

        template<typename FirstCmp, typename ... Cmps>
        CustomOrder(const FirstCmp& fcmp, const Cmps& ... cmps);

        bool operator()(const Monomial& a, const Monomial& b) const;

        int cmp(const Monomial& a, const Monomial& b) const;
    private:
        std::function<int (const Monomial&, const Monomial&)> compare;
    };

    using DefaultOrder = MonoLexOrder;

/*
=================================IMPLEMENTATION================================= 
*/

    template<typename FirstCmp, typename ... Cmps>
    CustomOrder::CustomOrder(const FirstCmp& fcmp, const Cmps& ... cmps)
        : CustomOrder(cmps...)
    {
        std::function<int (const Monomial&, const Monomial&)>& old_compare = compare;
        compare = [old_compare, fcmp](const Monomial& a, const Monomial& b) -> int {
            int res = fcmp(a, b);
            if (res == 0)
                return old_compare(a, b);
            return res;
        };
    }
}