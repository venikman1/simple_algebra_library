#include <iostream>
#include <cassert>
#include <string>
#include <boost/rational.hpp>

#include "polynomial.h"
#include "polynomial_set.h"
#include "algebra_io.h"
#include "tests.h"
#include "orders.h"
#include "algorithms.h"
#include "speed_tests.h"

using std::cout;
using std::cin;
using std::cerr;

using namespace SALIB;

struct Field {
    unsigned long long n;
    static const unsigned long long inv[5];
    Field():n(0) {}
    Field(unsigned long long n):n(n%5) {}
    Field& operator*=(const Field& other) { n*=other.n; n%=5; return *this;}
    Field& operator+=(const Field& other) { n+=other.n; n%=5; return *this;}
    Field& operator-=(const Field& other) { n+=(5 - other.n); n%=5; return *this;}
    Field& operator/=(const Field& other) { return (*this) *= Field(inv[other.n]);}

    Field operator*(const Field& other) const { Field res(*this); res *= other; return res;}
    Field operator+(const Field& other) const { Field res(*this); res += other; return res;}
    Field operator-(const Field& other) const { Field res(*this); res -= other; return res;}
    Field operator/(const Field& other) const { Field res(*this); res /= other; return res;}

    Field operator+() const { Field res(*this); return res;}
    Field operator-() const { Field res(*this); res.n = (5 - res.n) % 5; return res;}

    bool operator==(const Field& other) const {return n == other.n;} ;
    bool operator!=(const Field& other) const {return n != other.n;} ; 
};
const unsigned long long Field::inv[5] = {0, 1, 3, 2, 4};
std::ostream& operator<<(std::ostream& out, const Field& f) {return out << f.n;}
namespace std {
    template<>
    struct hash<Field> {
        size_t operator()(const Field& f) const {
            return f.n;
        }
    };
}

struct Kek1 {
    static int cmp(const Monomial&, const Monomial&) {
        cerr << "Hello "; return 0;
    }
};
struct Kek2 {
    static int cmp(const Monomial&, const Monomial&) {
        cerr << "world"; return 0;
    }
};
struct Kek3 {
    static int cmp(const Monomial&, const Monomial&) {
        cerr << "!\n"; return 0;
    }
};
int main() {
    #ifdef DEBUG
    test_all();
    #endif

    #ifdef SPEEDTEST
    use_all_pc_power();
    #endif

     cerr << "All tests succesful!\n";

     using CheckMe = CustomOrder<Kek1, Kek2, Kek3, MonoLexOrder>;

     Monomial a{1};
     Monomial b;
     cerr << CheckMe::cmp(a, b) << "\n";
     cerr << CheckMe::cmp(b, a) << "\n";
     cerr << CheckMe::cmp(a, a) << "\n";
     // You can play with polynomials there
     //                  |
     //                  V
     Polynomial<boost::rational<long long>> X = Monomial{1};
     Polynomial<boost::rational<long long>> Y = Monomial{0, 1};
     Polynomial<boost::rational<long long>> Z = Monomial{0, 0, 1};
     Polynomial<boost::rational<long long>> poly1 = X - boost::rational<long long>(1);
     Polynomial<boost::rational<long long>> poly2 = boost::rational<long long>(1) + X + X * X + X * X * X + X * X * X * X;
     cout << "poly1 = " << poly1 << "\n";
     cout << "LT(poly1) = " << Polynomial<long long>(poly1.get_largest_monomial()) << "\n";
     cout << "poly2 = " << poly2 << "\n";
     cout << "LT(poly1) = " << Polynomial<long long>(poly2.get_largest_monomial()) << "\n";
     cout << "poly1 * poly2 = " << poly1 * poly2 << "\n";
     cout << "LT(poly1 * poly2) = " << Polynomial<long long>((poly1 * poly2).get_largest_monomial()) << "\n";
     cout << "\n\nGroebner basis example:\n";
     using GrLex = CustomOrder<MonoGradientSemiOrder, MonoLexOrder>;

     // CustomOrder grlex(
     //     &MonoLexOrder::cmp
     // );

     // using CustomOrder = MonoLexOrder;
     // CustomOrder grlex;
     using Rat = boost::rational<long long>; //Field;
     using Poly = Polynomial<Rat, GrLex>;
     using PolySet = PolynomialSet<Rat, GrLex>;

     Poly x = Poly(Monomial{1});
     Poly y = Poly(Monomial{0, 1});
     Poly z = Poly(Monomial{0, 0, 1});
     Poly f1 = x * x * x - Rat(2) * x * y;
     Poly f2 = x * x * y - Rat(2) * y * y + x;
     PolySet ideal; ideal.add(f1); ideal.add(f2);
     PolySet basis = PolyAlg<Rat, GrLex>::make_groebner_basis(ideal);
     PolySet answer;
     cout << "Ideal:\n";
     for (const auto& poly : ideal) {
         cout << ">   " << poly << "\n";
     }
     cout << "\n";
     cout << "Basis:\n";
     for (const auto& poly : basis) {
         cout << ">   " << poly << "\n";
     }
     cout << "\n";
     Poly f = - Rat(2) * x * y;
     cout << "This is poly from ideal: " << f << "\n";
     cout << "This is reduction by ideal: " << PolyAlg<Rat, GrLex>::reduce_by(f, ideal) << "\n";
     cout << "This is reduction by basis: " << PolyAlg<Rat, GrLex>::reduce_by(f, basis) << "\n";
     cout << "\nIs f in ideal? Answer: " <<
         (PolyAlg<Rat, GrLex>::is_polynomial_in_ideal(f, ideal)?"YES":"NO") <<"\n";

     PolySet ideal1; ideal1.add(x * x - Rat(2) * x * z + z * z);
     f = x - z;
     Poly g = x;
     cout << "\nIdeal1 in which we want to find radical:\n";
     for (const auto& poly : ideal1) {
         cout << ">   " << poly << "\n";
     }
     cout << "f = " << f << "\n";
     cout << "g = " << g << "\n";
     cout << "\nIs f in radical? Answer: " <<
         (PolyAlg<Rat, GrLex>::is_polynomial_in_radical(f, ideal1, 3)?"YES":"NO") <<"\n";
     cout << "\nIs g in radical? Answer: " <<
         (PolyAlg<Rat, GrLex>::is_polynomial_in_radical(g, ideal1, 3)?"YES":"NO") <<"\n";

     PolySet ideal2; ideal2.add(y);
     cout << "\nIdeal2:\n";
     for (const auto& poly : ideal2) {
         cout << ">   " << poly << "\n";
     }
     cout << "\n";
     PolySet ideal_res = PolyAlg<Rat, GrLex>::intersect_ideals(ideal1, ideal2, 3);
     cout << "\nIdeal1 intersect Ideal2:\n";
     for (const auto& poly : ideal_res) {
         cout << ">   " << poly << "\n";
     }
     cout << "\n";
    return 0;
}