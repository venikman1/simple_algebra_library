#include <iostream>
#include <cassert>
#include <string>
#include <boost/rational.hpp>
#include <vector>

#include "polynomial.h"
#include "polynomial_set.h"
#include "algebra_io.h"
#include "tests.h"
#include "algorithms.h"

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
    c.zero();
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
    a.zero();
    c.zero();
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
    a.zero();
    c.zero();
    c[1] = 0;
    c[2] = 0;
    assert(order.cmp(a, c) == 0);
    assert(order.cmp(a * b, c * b) == 0);
    c[5] = 4;
    assert(order.cmp(a, c) < 0);
    assert(order.cmp(a * b, c * b) < 0);
    a[5] = 5;
    assert(order.cmp(a, c) > 0);
    assert(order.cmp(a * b, c * b) > 0);
    a[5] = 4;
    assert(order.cmp(a, c) == 0);
    assert(order.cmp(a * b, c * b) == 0);
    c[0] = 2;
    a[1] = 2;
    assert(order.cmp(a, c) < 0);
    assert(order.cmp(a * b, c * b) < 0);
    cerr << "Mono cmp tests OK!\n";

    // Mono divisions
    assert(a / a == Monomial());
    assert(a * b / a == b);
    assert(a.is_dividable_by(a) == true);
    assert(a.is_dividable_by(Monomial()) == true);
    a = Monomial({1, 2, 3, 0, 6});
    c = Monomial({0, 1, 1, 0, 2, 0, 0});
    assert(a.is_dividable_by(c) == true);
    c[3] = 2;
    assert(a.is_dividable_by(c) == false);

    Monomial d = Monomial::lcm(a, c);
    assert(d.is_dividable_by(a) == true);
    assert(d.is_dividable_by(c) == true);
    cerr << "Mono division tests OK!\n";

    // Mono divisions
}


void polynomial_tests() {
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

    // Largest monomial tests
    assert(
        a.get_largest_monomial_as_poly().get_largest_monomial() == a.get_largest_monomial()
    );
    b = a;
    a += Z * Z;
    assert(a.get_largest_monomial_as_poly() == b.get_largest_monomial_as_poly());
    assert(a.get_largest_monomial() == b.get_largest_monomial());
    b += 2LL * X * X * X * X;
    assert(b.get_largest_monomial() == Monomial({4, 0, 0, 0, 0, 0, 0, 0}));
    assert(b.get_largest_monomial() == Monomial({4, 0, 0, 0, 0, 0, 0, 0}));
    assert(b.get_largest_monomial_as_poly() == 2LL * X * X * X * X);
    assert(b.get_largest_monomial_as_poly() != X * X * X * X);
    cerr << "Getting largest monomial tests OK!\n";

    // Reduction tests
    using PolyTest = Polynomial<boost::rational<long long>>;
    using PolySet = PolynomialSet<boost::rational<long long>>;
    PolyTest x = Monomial{1};
    PolyTest y = Monomial{0, 1};
    PolyTest z = Monomial{0, 0, 1};
    PolySet s;
    PolyTest f1 = y * y - boost::rational<long long>(1);
    PolyTest f2 = x * y - boost::rational<long long>(1);
    PolyTest f3 = z * z;
    std::vector<PolyTest> sv;
    s.add(f1); s.add(f2); s.add(f3);
    sv.push_back(f1); sv.push_back(f2); sv.push_back(f3);
    PolyTest f = x * x * y + x * y * y + y * y + z * z * z - x * y * z + z * z * x * y * y * x;
    std::vector<PolyTest> answer;
    PolyTest r = PolyAlg<boost::rational<long long>, MonoLexOrder>::reduce_by(f, sv, &answer);
    assert(answer[0] * f1 + answer[1] * f2 + answer[2] * f3 + r == f);
    for (auto& it : r) {
        assert(!it.first.is_dividable_by(f1.get_largest_monomial()));
        assert(!it.first.is_dividable_by(f1.get_largest_monomial()));
        assert(!it.first.is_dividable_by(f1.get_largest_monomial()));
    }
    cerr << "Reduction tests OK!\n";

    // S polynomial tests
    f1 = x * x * y + y * z;
    f2 = x * y + z;

    assert(PolyTest::s_polynomial(f1, f2) == y * z - x * z);
    assert(PolyTest::s_polynomial(f1, f2) == -PolyTest::s_polynomial(f2, f1));
    cerr << "S polynomial tests OK!\n";
}

void test_all() {
    polynomial_tests();
    monomial_tests();
}
