#pragma once
#include <iostream>
#include <boost/functional/hash.hpp>

namespace SALIB {
    const int N = 2;

    struct Field {
        unsigned long long n;
        static const unsigned long long inv[N];

        Field() : n(0) {}

        Field(unsigned long long n) : n((n + N) % N) {}

        Field &operator*=(const Field &other) {
            n *= other.n;
            n %= N;
            return *this;
        }

        Field &operator+=(const Field &other) {
            n += other.n;
            n %= N;
            return *this;
        }

        Field &operator-=(const Field &other) {
            n += (N - other.n);
            n %= N;
            return *this;
        }

        Field &operator/=(const Field &other) { return (*this) *= Field(inv[other.n]); }

        Field operator*(const Field &other) const {
            Field res(*this);
            res *= other;
            return res;
        }

        Field operator+(const Field &other) const {
            Field res(*this);
            res += other;
            return res;
        }

        Field operator-(const Field &other) const {
            Field res(*this);
            res -= other;
            return res;
        }

        Field operator/(const Field &other) const {
            Field res(*this);
            res /= other;
            return res;
        }

        Field operator+() const {
            Field res(*this);
            return res;
        }

        Field operator-() const {
            Field res(*this);
            res.n = (N - res.n) % N;
            return res;
        }

        bool operator==(const Field &other) const { return n == other.n; };

        bool operator!=(const Field &other) const { return n != other.n; };

        friend size_t hash_value(const SALIB::Field &f) {
            return f.n;
        }
    };

    const unsigned long long Field::inv[N] = {0, 1};// , 3, 2, 4};

    std::ostream &operator<<(std::ostream &out, const Field &f) { return out << f.n; }



}

namespace boost {


}