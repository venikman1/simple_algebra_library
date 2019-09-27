#pragma once
#include <iostream>
#include <boost/functional/hash.hpp>

namespace SALIB {

    template <int N = 2>
    struct Field {
        unsigned long long n;
        static const unsigned long long inv[N];

        Field() : n(0) {}

        Field(unsigned long long n) : n((n + N) % N) {}

        inline Field &operator*=(const Field &other) {
            n *= other.n;
            n %= N;
            return *this;
        }

        inline Field &operator+=(const Field &other) {
            n += other.n;
            n %= N;
            return *this;
        }

        inline Field &operator-=(const Field &other) {
            n += (N - other.n);
            n %= N;
            return *this;
        }

        inline Field &operator/=(const Field &other) { return (*this) *= Field(inv[other.n]); }

        inline Field operator*(const Field &other) const {
            Field res(*this);
            res *= other;
            return res;
        }

        inline Field operator+(const Field &other) const {
            Field res(*this);
            res += other;
            return res;
        }

        inline Field operator-(const Field &other) const {
            Field res(*this);
            res -= other;
            return res;
        }

        inline Field operator/(const Field &other) const {
            Field res(*this);
            res /= other;
            return res;
        }

        inline Field operator+() const {
            Field res(*this);
            return res;
        }

        inline Field operator-() const {
            Field res(*this);
            res.n = (N - res.n) % N;
            return res;
        }

        inline bool operator==(const Field &other) const { return n == other.n; };

        inline bool operator!=(const Field &other) const { return n != other.n; };

        inline friend size_t hash_value(const Field &f) {
            return f.n;
        }
    };

    template <int N>
    const unsigned long long Field<N>::inv[N] = {0, 1};// , 3, 2, 4};

    template <int N>
    inline std::ostream &operator<<(std::ostream &out, const Field<N> &f) { return out << f.n; }



}
