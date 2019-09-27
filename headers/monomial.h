#pragma once

#include <vector>
#include <algorithm>
#include "orders.h"

namespace SALIB {

    class Monomial {
    public:
        using VariableIndexType = size_t;
        using VariableDegreeType = unsigned long long;
        using VariablesContainer = std::vector<VariableDegreeType>;
        using iterator = VariablesContainer::iterator;
        using const_iterator = VariablesContainer::const_iterator;
        using reverse_iterator = VariablesContainer::reverse_iterator;
        using const_reverse_iterator = VariablesContainer::const_reverse_iterator;


        inline Monomial() = default;
        inline Monomial(VariableIndexType var_index, VariableDegreeType var_degree = 1);
        inline Monomial(const std::initializer_list<VariableDegreeType>& init_list);

        inline static Monomial lcm(const Monomial& a, const Monomial& b);

        inline Monomial& operator*=(const Monomial& other);
        inline Monomial operator*(const Monomial& other) const;
        inline Monomial& operator/=(const Monomial& other);
        inline Monomial operator/(const Monomial& other) const;

        inline bool is_dividable_by(const Monomial& other) const;

        inline VariableDegreeType operator[](VariableIndexType var_index) const;

        inline void zero_all_powers();
        inline bool is_zero() const;
        inline bool operator==(const Monomial& other) const;
        inline bool operator<(const Monomial& other) const;
        inline bool operator!=(const Monomial& other) const;

        inline bool is_depends_on_variable(VariableIndexType var_index) const;

        inline const_iterator begin() const;
        inline const_iterator end() const;
        inline const_reverse_iterator rbegin() const;
        inline const_reverse_iterator rend() const;

        inline void set_var_degree(VariableIndexType var, VariableDegreeType deg);

        inline VariableDegreeType get_degree() const;
    private:
        inline VariableDegreeType& operator[](VariableIndexType var_index);

        VariablesContainer variables;
        VariableDegreeType degree = 0;
    };

    Monomial::VariableDegreeType Monomial::get_degree() const {
        return degree;
    }

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
            : variables(init_list) {
        degree = 0;
        for (auto deg : variables)
            degree += deg;
    }

    Monomial::Monomial(VariableIndexType var_index, VariableDegreeType var_degree) {
        (*this)[var_index] = var_degree;
        degree = var_degree;
    }

    Monomial Monomial::lcm(const Monomial& a, const Monomial& b) {
        Monomial res(a);
        for (size_t i = 0; i < b.variables.size(); ++i) {
            res[i] = std::max(res[i], b[i]);
            res.degree += std::max(res[i], b[i]) - res[i];
        }
        return res;
    }

    Monomial& Monomial::operator*=(const Monomial& other) {
        for (size_t i = 0; i < other.variables.size(); ++i) {
            (*this)[i] += other[i];
            degree += other[i];
        }
        return *this;
    }

    Monomial Monomial::operator*(const Monomial& other) const {
        Monomial res(*this);
        return res *= other;
    }

    Monomial& Monomial::operator/=(const Monomial& other) {
        for (size_t i = 0; i < other.variables.size(); ++i) {
            (*this)[i] -= other[i];
            degree -= other[i];
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
        return degree == 0;
//        for (const auto& pw : variables) {
//            if (pw != 0)
//                return false;
//        }
//        return true;
    }

    Monomial::VariableDegreeType& Monomial::operator[](VariableIndexType var_index) {
        while (variables.size() <= var_index)
            variables.push_back(0);
        return variables[var_index];
    }

    Monomial::VariableDegreeType Monomial::operator[](VariableIndexType var_index) const {
        if (var_index >= variables.size())
            return VariableDegreeType(0);
        return variables[var_index];
    }

    void Monomial::zero_all_powers() {
        variables.clear();
        degree = 0;
    }

    Monomial::const_iterator Monomial::begin() const {
        return variables.begin();
    }

    Monomial::const_iterator Monomial::end() const {
        return variables.end();
    }

    Monomial::const_reverse_iterator Monomial::rbegin() const {
        return variables.rbegin();
    }

    Monomial::const_reverse_iterator Monomial::rend() const {
        return variables.rend();
    }

    void Monomial::set_var_degree(VariableIndexType var, VariableDegreeType deg) {
        degree += deg - (*this)[var];
        (*this)[var] = deg;
    }
}

#include "order_impl.h"