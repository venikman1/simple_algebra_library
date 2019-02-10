#include "monomial.h"
#include "orders.h"

namespace SALIB {
    bool Monomial::operator==(const Monomial& other) const {
        return MonoLexOrder::mono_lex_cmp(*this, other) == 0;
    }

    bool Monomial::operator<(const Monomial& other) const {
        return MonoLexOrder::mono_lex_cmp(*this, other) < 0;
    }

    bool Monomial::operator!=(const Monomial& other) const {
        return !(*this == other);
    }
}