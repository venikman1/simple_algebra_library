#pragma once
#include <map>

namespace SALIB {

    class Monomial {
    public:
        using VariableIndexType = long long;
        using VariableDegreeType = unsigned long long;
        using VariablesContainer = std::map<VariableIndexType, VariableDegreeType>;
        using iterator = VariablesContainer::iterator;
        using const_iterator = VariablesContainer::const_iterator;
        using reverse_iterator = VariablesContainer::reverse_iterator;
        using const_reverse_iterator = VariablesContainer::const_reverse_iterator;
    
        Monomial() = default;

        /* TODO: make named constructors */
        Monomial(const std::initializer_list<long long>& init_list) {
            long long key = 0;
            for (long long value : init_list) {
                variables[key++] = value;
            }
        }

        Monomial& operator*=(const Monomial& other) {
            for (const auto& it : other.variables) {
                variables[it.first] += it.second;
            }
            return *this;
        }

        Monomial operator*(const Monomial& other) const {
            Monomial res(*this);
            return res *= other;
        }

        VariableDegreeType& operator[](VariableIndexType var_index) {
            return variables[var_index];
        }

        VariableDegreeType operator[](VariableIndexType var_index) const {
            auto found = variables.find(var_index);
            if (found == variables.end())
                return VariableDegreeType(0);
            return found->second;
        }

        void clean_empty_variables() {
            for (auto it = variables.begin(); it != variables.end();) {
                auto next_it = it;
                ++next_it;
                if (it->second == 0)
                    variables.erase(it);
                it = next_it;
            }
        }

        void clear() {
            variables.clear();
        }

        bool operator==(const Monomial& other) const;

        bool operator<(const Monomial& other) const;

        bool operator!=(const Monomial& other) const;

        iterator begin() {
            return variables.begin();
        }

        iterator end() {
            return variables.end();
        }

        const_iterator begin() const {
            return variables.begin();
        }

        const_iterator end() const {
            return variables.end();
        }

        reverse_iterator rbegin() {
            return variables.rbegin();
        }

        reverse_iterator rend() {
            return variables.rend();
        }

        const_reverse_iterator rbegin() const {
            return variables.rbegin();
        }

        const_reverse_iterator rend() const {
            return variables.rend();
        }

    private:
        VariablesContainer variables;
    };
}