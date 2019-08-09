#pragma once

#include <vector>
#include <algorithm>

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


        Monomial() = default;
        Monomial(VariableIndexType var_index, VariableDegreeType var_degree = 1);
        Monomial(const std::initializer_list<VariableDegreeType>& init_list);

        static Monomial lcm(const Monomial& a, const Monomial& b);

        Monomial& operator*=(const Monomial& other);
        Monomial operator*(const Monomial& other) const;
        Monomial& operator/=(const Monomial& other);
        Monomial operator/(const Monomial& other) const;

        bool is_dividable_by(const Monomial& other) const;

        VariableDegreeType& operator[](VariableIndexType var_index);
        VariableDegreeType operator[](VariableIndexType var_index) const;

        void zero_all_powers();
        bool is_zero() const;
        bool operator==(const Monomial& other) const;
        bool operator<(const Monomial& other) const;
        bool operator!=(const Monomial& other) const;

        bool is_depends_on_variable(VariableIndexType var_index) const;

        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;
        reverse_iterator rbegin();
        reverse_iterator rend();
        const_reverse_iterator rbegin() const;
        const_reverse_iterator rend() const;

    private:
        VariablesContainer variables;
    };
}