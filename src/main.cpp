#include <iostream>
#include <cassert>
#include <string>
#include <boost/rational.hpp>

#include "polynomial.h"
#include "polynomial_set.h"
#include "algebra_io.h"
#include "tests.h"
#include "orders.h"

using std::cout;
using std::cin;
using std::cerr;

using namespace SALIB;

int main() {
    test_all();
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
    // Polynomial<boost::rational<long long>> X = Monomial{1};
    // Polynomial<boost::rational<long long>> Y = Monomial{0, 1};
    // Polynomial<boost::rational<long long>> Z = Monomial{0, 0, 1};
    // Polynomial<boost::rational<long long>> poly1 = X - boost::rational<long long>(1);
    // Polynomial<boost::rational<long long>> poly2 = boost::rational<long long>(1) + X + X * X + X * X * X + X * X * X * X;
    // cout << "poly1 = " << poly1 << "\n";
    // cout << "LT(poly1) = " << Polynomial<long long>(poly1.get_largest_monomial()) << "\n";
    // cout << "poly2 = " << poly2 << "\n";
    // cout << "LT(poly1) = " << Polynomial<long long>(poly2.get_largest_monomial()) << "\n";
    // cout << "poly1 * poly2 = " << poly1 * poly2 << "\n";
    // cout << "LT(poly1 * poly2) = " << Polynomial<long long>((poly1 * poly2).get_largest_monomial()) << "\n";
    // cout << "\n\nGroebner basis example:\n";
    // using Poly = Polynomial<boost::rational<long long>>;
    // using PolySet = PolynomialSet<boost::rational<long long>>;
    // using Rat = boost::rational<long long>;
    // Poly x = Monomial{1}, y = Monomial{0, 1}, z = Monomial{0, 0, 1};
    // Poly f1 = x * x * x - Rat(2) * x * y;
    // Poly f2 = x * x * y - Rat(2) * y * y + x;
    // PolySet ideal; ideal.add(f1); ideal.add(f2);
    // PolySet basis = ideal.get_groebner_basis();
    // PolySet answer;
    // cout << "Ideal:\n";
    // for (const auto& poly : ideal) {
    //     cout << ">   " << poly << "\n"; 
    // }
    // cout << "\n";
    // cout << "Basis:\n";
    // for (const auto& poly : basis) {
    //     cout << ">   " << poly << "\n"; 
    // }
    // cout << "\n";
    // Poly f = - Rat(2) * x * y;
    // cout << "This is poly from ideal: " << f << "\n";
    // cout << "This is reduction by ideal: " << ideal.reduce(f, answer) << "\n";
    // cout << "This is reduction by basis: " << basis.reduce(f, answer) << "\n";
    return 0;
}