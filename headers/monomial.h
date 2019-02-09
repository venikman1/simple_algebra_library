#pragma once
#include <map>
#include <iostream>

namespace SALIB {

    class Monomial
    {
        using VarContainer = std::map<long long, long long>;
        using iterator = VarContainer::iterator;
        using const_iterator = VarContainer::const_iterator;
        using reverse_iterator = VarContainer::reverse_iterator;
        using const_reverse_iterator = VarContainer::const_reverse_iterator;

    private:
        VarContainer variables;
        
    public:
        Monomial() = default;

        Monomial(const std::initializer_list<long long>& init_list) {
            long long key = 0;
            for (long long value : init_list) {
                variables[key++] = value;
            }
        }

        Monomial& operator*=(const Monomial& other) {
            for (auto& it : other.variables) {
                variables[it.first] += it.second;
            }
            return *this;
        }

        Monomial operator*(const Monomial& other) const {
            Monomial res(*this);
            return res *= other;
        }

        long long& operator[](long long key) {
            return variables[key];
        }

        long long operator[](long long key) const {
            auto found = variables.find(key);
            if (found == variables.end())
                return 0;
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

        bool operator==(const Monomial& other) const {
            auto it1 = variables.begin();
            auto it2 = other.variables.begin();
            while (it1 != variables.end() || it2 != other.variables.end()) {
                while (it1->second == 0 && it1 != variables.end())
                    ++it1;
                while (it2->second == 0 && it2 != other.variables.end())
                    ++it2;
                if (it2 == other.variables.end() && it1 == variables.end())
                    return true;
                if (it2 == other.variables.end() || it1 == variables.end())
                    return false;
                if (it1->first != it2->first)
                    return false;
                if (it1->second != it2->second)
                    return false;
                ++it1;
                ++it2;
            }
            return true;
        }

        bool operator!=(const Monomial& other) const {
            return !(*this == other);
        }

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
    };
}