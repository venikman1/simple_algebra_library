#pragma once

#include <functional>
#include <utility>


namespace SALIB {
    class Monomial;   // Me: C++, this class exists, trust me
                      // C++: I believe you, but you can't use methods
                      // Me: WHY? WHY? Can u just add normal cyclic import pls
                      // C++: Haha, no
                      // Me: ...

    class MonoLexOrder {
    public:
        inline static int cmp(const Monomial& a, const Monomial& b);

        inline bool operator()(const Monomial& a, const Monomial& b) const;
    };

    class MonoGradientSemiOrder {
    public:
        inline static int cmp(const Monomial& a, const Monomial& b);

        inline bool operator()(const Monomial& a, const Monomial& b) const;
    };

    template <typename Order>
    class RevOrder {
    public:
        inline static int cmp(const Monomial& a, const Monomial& b);

        inline bool operator()(const Monomial& a, const Monomial& b) const;
    };

    template <typename FirstOrder, typename ... Orders>
    class CustomOrder {
    public:
        inline bool operator()(const Monomial& a, const Monomial& b) const;

        inline static int cmp(const Monomial& a, const Monomial& b);
    };

    template <typename FirstOrder>
    class CustomOrder<FirstOrder> {
    public:
        inline static int cmp(const Monomial& a, const Monomial& b);
    };

    using DefaultOrder = MonoLexOrder;
}