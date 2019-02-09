#include <iostream>
#include <cassert>
#include "polynomial.h"

using std::cout;
using std::cin;
using std::cerr;

using namespace SALIB;

void monomial_tests() {
    // Arithmetic tests
    Monomial a; 
    a[0] = 2; a[1] = 4;
    const Monomial b{0, 0, 3};
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
    cerr << "Order tests OK!\n";
}


int main() {
    monomial_tests();
    cout << "All tests succesful!\n";
    return 0;
}