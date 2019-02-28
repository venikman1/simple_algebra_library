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

    class CustomOrder {
    public:
        CustomOrder() {
            compare = [](const Monomial& a, const Monomial& b) -> int {return 0;};
        }

        template<typename FirstCmp, typename ... Cmps>
        CustomOrder(const FirstCmp& fcmp, const Cmps& ... cmps)
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

        bool operator()(const Monomial& a, const Monomial& b) const {
            return compare(a, b) < 0;
        }

        int cmp(const Monomial& a, const Monomial& b) const {
            return compare(a, b);
        }

    private:
        std::function<int (const Monomial&, const Monomial&)> compare;
    };
}