#pragma once
#include <map>
#include <algorithm>

namespace SALIB {

    class Monomial {
    public:
        using VariableIndexType = unsigned long long;
        using VariableDegreeType = long long;
        using VariablesContainer = std::map<VariableIndexType, VariableDegreeType>;
        using iterator = VariablesContainer::iterator;
        using const_iterator = VariablesContainer::const_iterator;
        using reverse_iterator = VariablesContainer::reverse_iterator;
        using const_reverse_iterator = VariablesContainer::const_reverse_iterator;
    
        Monomial() = default;

        Monomial(const std::initializer_list<long long>& init_list);

        static Monomial lcm(const Monomial& a, const Monomial& b);

        Monomial& operator*=(const Monomial& other);
        Monomial operator*(const Monomial& other) const;
        Monomial& operator/=(const Monomial& other);
        Monomial operator/(const Monomial& other) const;

        bool is_dividable_by(const Monomial& other) const;

        VariableDegreeType& operator[](VariableIndexType var_index);
        VariableDegreeType operator[](VariableIndexType var_index) const;

        void zero();

        bool operator==(const Monomial& other) const;
        bool operator<(const Monomial& other) const;
        bool operator!=(const Monomial& other) const;

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