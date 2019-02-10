#include <iostream>
#include <cassert>
#include <string>
#include <regex>
#include "polynomial.h"
#include "algebra_io.h"

using std::cout;
using std::cin;
using std::cerr;

using namespace SALIB;

void monomial_tests() {
    // Arithmetic tests
    Monomial a; 
    a[0] = 2; a[1] = 4;
    const Monomial b{0, 0, 3, 5, 1, 2};
    Monomial c;
    c = a * b;
    assert(c == a * b);
    c.clear();
    assert(c == Monomial());
    assert(c != a);
    assert(a * c == a);

    Monomial a_copy(a);
    assert(a == a_copy);
    a[5324] = 0;
    assert(a == a_copy);
    ++a[12];
    assert(a != a_copy);
    cerr << "Monomial arithmetic tests OK!\n";

    // Order tests
    MonoLexOrder order;
    a.clear();
    c.clear();
    a[1] = 2;
    c[1] = 2;
    assert(order(a, c) == false);
    c[5] = 0;
    assert(order(a, c) == false);
    c[3] = 5;
    assert(order(a, c) == true);
    a[0] = 2;
    assert(order(a, c) == false);
    c[0] = 2;
    assert(order(a, c) == true);
    a[2] = 0;
    assert(order(a, c) == true);
    cerr << "Mono order tests OK!\n";

    // Mono cmp tests
    a.clear();
    c.clear();
    c[1] = 0;
    c[2] = 0;
    assert(order.mono_lex_cmp(a, c) == 0);
    assert(order.mono_lex_cmp(a * b, c * b) == 0);
    c[5] = 4;
    assert(order.mono_lex_cmp(a, c) < 0);
    assert(order.mono_lex_cmp(a * b, c * b) < 0);
    a[5] = 5;
    assert(order.mono_lex_cmp(a, c) > 0);
    assert(order.mono_lex_cmp(a * b, c * b) > 0);
    a[5] = 4;
    assert(order.mono_lex_cmp(a, c) == 0);
    assert(order.mono_lex_cmp(a * b, c * b) == 0);
    c[0] = 2;
    a[1] = 2;
    assert(order.mono_lex_cmp(a, c) < 0);
    assert(order.mono_lex_cmp(a * b, c * b) < 0);
    cerr << "Mono cmp tests OK!\n";
}


void polymial_tests() {
    // Arithmetic tests
    Polynomial<long long> X = Monomial{1};
    Polynomial<long long> Y = Monomial{0, 1};
    Polynomial<long long> Z = Monomial{0, 0, 1};
    Polynomial<long long> a = X * Y + Z * 4LL - 23LL * Y * Y * Y;
    Polynomial<long long> a_after_cast = Polynomial<double>(a);
    assert(a == a_after_cast);
    
    a_after_cast += X * Y * Z;
    Polynomial<long long> b = a - a_after_cast;
    assert(b == -X * Y * Z);
    assert(b == b);
    assert(b * a != a * a);
    assert(b * a == -X * Y * Z * a);
    assert(b + a == -X * Y * Z + a);
    assert(b + Polynomial<long long>() == b);
    assert(b * Polynomial<long long>() == Polynomial<long long>());
    cerr << "Poly arithmetic tests OK!\n";
}
int main() {
    monomial_tests();
    polymial_tests();
    cerr << "All tests succesful!\n";

    // You can play with polynomials there
    //                  |
    //                  V
    Polynomial<long long> X = Monomial{1};
    Polynomial<long long> Y = Monomial{0, 1};
    Polynomial<long long> Z = Monomial{0, 0, 1};
    Polynomial<long long> poly1 = 2LL * X * X + Z;
    Polynomial<long long> poly2 = Y + Z * Z;
    cout << "poly1 = " << poly1 << "\n";
    cout << "LT(poly1) = " << Polynomial<long long>(poly1.get_largest_monomial()) << "\n";
    cout << "poly2 = " << poly2 << "\n";
    cout << "LT(poly1) = " << Polynomial<long long>(poly2.get_largest_monomial()) << "\n";
    cout << "poly1 * poly2 = " << poly1 * poly2 << "\n";
    cout << "LT(poly1 * poly2) = " << Polynomial<long long>((poly1 * poly2).get_largest_monomial()) << "\n";
    return 0;
}