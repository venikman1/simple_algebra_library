#include "monomial.h"
#include "orders.h"
#include "polynomial.h"

namespace SALIB {
    bool Monomial::operator==(const Monomial& other) const {
        return MonoLexOrder::cmp(*this, other) == 0;
    }

    bool Monomial::operator<(const Monomial& other) const {
        return MonoLexOrder::cmp(*this, other) < 0;
    }

    bool Monomial::operator!=(const Monomial& other) const {
        return !(*this == other);
    }

    Monomial::Monomial(const std::initializer_list<long long>& init_list) {
        long long key = 0;
        for (long long value : init_list) {
            if (value)
                variables[key] = value;
            ++key;
        }
    }

    Monomial Monomial::lcm(const Monomial& a, const Monomial& b) {
        Monomial res = a;
        for (const auto& it : b) {
            res[it.first] = std::max(res[it.first], it.second);
        }
        return res;
    }

    Monomial& Monomial::operator*=(const Monomial& other) {
        for (const auto& it : other.variables) {
            variables[it.first] += it.second;
        }
        return *this;
    }

    Monomial Monomial::operator*(const Monomial& other) const {
        Monomial res(*this);
        return res *= other;
    }

    Monomial& Monomial::operator/=(const Monomial& other) {
        for (const auto& it : other.variables) {
            variables[it.first] -= it.second;
        }
        return *this;
    }

    Monomial Monomial::operator/(const Monomial& other) const {
        Monomial res(*this);
        return res /= other;
    }

    bool Monomial::is_dividable_by(const Monomial& other) const {
        for (const auto& it : other) {
            if ((*this)[it.first] < it.second)
                return false;
        }
        return true;
    }

    Monomial::VariableDegreeType& Monomial::operator[](VariableIndexType var_index) {
        return variables[var_index];
    }

    Monomial::VariableDegreeType Monomial::operator[](VariableIndexType var_index) const {
        auto found = variables.find(var_index);
        if (found == variables.end())
            return VariableDegreeType(0);
        return found->second;
    }

    void Monomial::zero() {
        variables.clear();
    }

    Monomial::iterator Monomial::begin() {
        return variables.begin();
    }

    Monomial::iterator Monomial::end() {
        return variables.end();
    }

    Monomial::const_iterator Monomial::begin() const {
        return variables.begin();
    }

    Monomial::const_iterator Monomial::end() const {
        return variables.end();
    }

    Monomial::reverse_iterator Monomial::rbegin() {
        return variables.rbegin();
    }

    Monomial::reverse_iterator Monomial::rend() {
        return variables.rend();
    }

    Monomial::const_reverse_iterator Monomial::rbegin() const {
        return variables.rbegin();
    }

    Monomial::const_reverse_iterator Monomial::rend() const {
        return variables.rend();
    }
}