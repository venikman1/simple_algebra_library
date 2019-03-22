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
std::ostream& operator<<(std::ostream& out, const Field& f) {out << f.n;}
namespace std {
    template<>
    struct hash<Field> {
        size_t operator()(const Field& f) const {
            return f.n;
        }
    };
}

int main() {
    #ifdef DEBUG
    test_all();
    #endif

    cerr << "All tests succesful!\n";

    CustomOrder check_me(
        [](const Monomial&, const Monomial&) -> int {cerr << "Hello "; return 0;},
        [](const Monomial&, const Monomial&) -> int {cerr << "world"; return 0;},
        [](const Monomial&, const Monomial&) -> int {cerr << "!\n"; return 0;},
        &MonoLexOrder::cmp
    );
    
    Monomial a{1};
    Monomial b;
    cerr << check_me.cmp(a, b) << "\n";
    cerr << check_me.cmp(b, a) << "\n";
    cerr << check_me.cmp(a, a) << "\n";
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
    CustomOrder grlex(
        &GradientSemiOrder::cmp,
        &MonoLexOrder::cmp
    );

    // CustomOrder grlex(
    //     &MonoLexOrder::cmp
    // );

    // using CustomOrder = MonoLexOrder;
    // CustomOrder grlex;
    using Rat = boost::rational<long long>; //Field;
    using Poly = Polynomial<Rat, CustomOrder>;
    using PolySet = PolynomialSet<Rat, CustomOrder>;
    
    Poly x = Poly(Monomial{1}, Rat(1), grlex);
    Poly y = Poly(Monomial{0, 1}, Rat(1), grlex);
    Poly z = Poly(Monomial{0, 0, 1}, Rat(1), grlex);
    Poly f1 = x * x * x - Rat(2) * x * y;
    Poly f2 = x * x * y - Rat(2) * y * y + x;
    PolySet ideal; ideal.add(f1); ideal.add(f2);
    PolySet basis = PolyAlg<Rat, CustomOrder>::make_groebner_basis(ideal, grlex);
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
    cout << "This is reduction by ideal: " << PolyAlg<Rat, CustomOrder>::reduce_by(f, ideal, grlex) << "\n";
    cout << "This is reduction by basis: " << PolyAlg<Rat, CustomOrder>::reduce_by(f, basis, grlex) << "\n";
    cout << "\nIs f in ideal? Answer: " << 
        (PolyAlg<Rat, CustomOrder>::is_polynomial_in_ideal(f, ideal, grlex)?"YES":"NO") <<"\n";
    
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
        (PolyAlg<Rat, CustomOrder>::is_polynomial_in_radical(f, ideal1, 3, grlex)?"YES":"NO") <<"\n";
    cout << "\nIs g in radical? Answer: " << 
        (PolyAlg<Rat, CustomOrder>::is_polynomial_in_radical(g, ideal1, 3, grlex)?"YES":"NO") <<"\n";
    
    PolySet ideal2; ideal2.add(y);
    cout << "\nIdeal2:\n";
    for (const auto& poly : ideal2) {
        cout << ">   " << poly << "\n"; 
    }
    cout << "\n";
    PolySet ideal_res = PolyAlg<Rat, CustomOrder>::intersect_ideals(ideal1, ideal2, 3, grlex);
    cout << "\nIdeal1 intersect Ideal2:\n";
    for (const auto& poly : ideal_res) {
        cout << ">   " << poly << "\n"; 
    }
    cout << "\n";
    return 0;
}