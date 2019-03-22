#include "monomial.h"
#include "orders.h"
#include "polynomial.h"

#include <cassert>

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

    Monomial::Monomial(const std::initializer_list<VariableDegreeType>& init_list)
        : variables(init_list) {}

    Monomial::Monomial(VariableIndexType var_index, VariableDegreeType var_degree) {
        (*this)[var_index] = var_degree;
    }

    Monomial Monomial::lcm(const Monomial& a, const Monomial& b) {
        Monomial res(a);
        for (size_t i = 0; i < b.variables.size(); ++i) {
            res[i] = std::max(res[i], b[i]);
        }
        return res;
    }

    Monomial& Monomial::operator*=(const Monomial& other) {
        for (size_t i = 0; i < other.variables.size(); ++i) {
            (*this)[i] += other[i];
        }
        return *this;
    }

    Monomial Monomial::operator*(const Monomial& other) const {
        Monomial res(*this);
        return res *= other;
    }

    Monomial& Monomial::operator/=(const Monomial& other) {
        for (size_t i = 0; i < other.variables.size(); ++i) {
            #ifdef DEBUG
            assert((*this)[i] >= other[i]);
            #endif
            (*this)[i] -= other[i];
        }
        return *this;
    }

    Monomial Monomial::operator/(const Monomial& other) const {
        Monomial res(*this);
        return res /= other;
    }

    bool Monomial::is_dividable_by(const Monomial& other) const {
        for (size_t i = 0; i < other.variables.size(); ++i) {
            if ((*this)[i] < other[i])
                return false;
        }
        return true;
    }

    bool Monomial::is_depends_on_variable(VariableIndexType var_index) const {
        return (*this)[var_index] != 0;
    }

    bool Monomial::is_zero() const {
        for (const auto& pw : variables) {
            if (pw != 0)
                return false;
        }
        return true;
    }

    Monomial::VariableDegreeType& Monomial::operator[](VariableIndexType var_index) {
        if (var_index >= variables.size())
            variables.resize(var_index + 1);
        return variables[var_index];
    }

    Monomial::VariableDegreeType Monomial::operator[](VariableIndexType var_index) const {
        if (var_index >= variables.size())
            return VariableDegreeType(0);
        return variables[var_index];
    }

    void Monomial::zero_all_powers() {
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