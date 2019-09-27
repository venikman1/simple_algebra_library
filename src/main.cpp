#include <iostream>
#include <cassert>
#include <string>
#include <boost/rational.hpp>
#include <thread>
#include <future>
#include <mutex>

#include "polynomial.h"
#include "polynomial_set.h"
#include "algebra_io.h"
#include "tests.h"
#include "orders.h"
#include "algorithms.h"
#include "speed_tests.h"
#include "stopwatch.h"
#include <boost/multiprecision/gmp.hpp>

using std::cout;
using std::cin;
using std::cerr;

using namespace SALIB;



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
void see_what_i_can() {
    test_all();

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
}


std::mutex cout_mutex;

int main() {
    using CoefType = Field<>; // boost::multiprecision::mpq_rational;
    auto lex_test = [](const PolynomialSet<CoefType>& idl) -> double {
        StopWatch watch;
        // using CoefType = Field;
        
        using Order = MonoLexOrder;
        PolynomialSet<CoefType, Order> ideal(idl);
        auto basis = SpeedTest::calc_basis_and_reduce(ideal);
        cerr << "Lex test ended\n";
        std::lock_guard<std::mutex> guard(cout_mutex);
        cout << "Lex order:" << watch.get_duration() << "\n";
        cout.flush();
        return watch.get_duration();
    };
    auto deglex_test = [](const PolynomialSet<CoefType>& idl) -> double {
        StopWatch watch;
        using Order = CustomOrder<MonoGradientSemiOrder, MonoLexOrder>;
        PolynomialSet<CoefType, Order> ideal(idl);
        auto basis = SpeedTest::calc_basis_and_reduce(ideal);
        cerr << "DegLex test ended\n";
        std::lock_guard<std::mutex> guard(cout_mutex);
        cout << "DegLex order:" << watch.get_duration() << "\n";
        cout.flush();
        
        return watch.get_duration();
    };

    auto degrevlex_test = [](const PolynomialSet<CoefType>& idl) -> double {
        StopWatch watch;
        using Order = CustomOrder<MonoGradientSemiOrder, RevOrder<MonoLexOrder>>;
        PolynomialSet<CoefType, Order> ideal(idl);
        // cerr << "IDEAL DEGREVLEX:\n";
        // for (const auto& poly : ideal) {
        //     cerr << "> " << poly << "\n";
        //     for (const auto& mono : poly) {
        //         cerr << "mono > " << mono.first << "\n";
        //         cerr << "is_zero > " << mono.first.is_zero() << "\n";
        //         cerr << "coeff > " << mono.second << "\n";
        //     }
        // }
        auto basis = SpeedTest::calc_basis_and_reduce(ideal);
        cerr << "DegRevLex test ended\n";
        std::lock_guard<std::mutex> guard(cout_mutex);
        cout << "DegRevLex order:" << watch.get_duration() << "\n";
        cout.flush();
        // for (const auto& poly : basis) {
        //     cerr << "> " << poly << "\n";
        // }
        return watch.get_duration();
    };
    
    
    using Poly = Polynomial<CoefType, DefaultOrder>;
    using PolySet = PolynomialSet<CoefType, DefaultOrder>;

    PolySet input_ideal = SpeedTest::read_polyset<CoefType, DefaultOrder>(cin);
    
    cerr << "Input ideal\n";
    for (const auto& poly : input_ideal) {
        cerr << "> " << poly << "\n";
    }
    // PolySet ideal;
    // int n = 7;
    // for (int idx = 1; idx < n; ++idx) {
    //     ideal.add(SpeedTest::get_symmetric_k<CoefType, DefaultOrder>(n, idx));
    // }

    // ideal.add(SpeedTest::get_symmetric_k<CoefType, DefaultOrder>(n, n) - Poly(CoefType((n % 2 == 0) ? -1 : 1)));    
    
    
    // cerr << "generated ideal:\n";
    // for (const auto& poly : ideal) {
    //     cerr << "> " << poly << "\n";
    // }

    // for (const auto& poly : ideal) {
    //     if (!input_ideal.contains(poly))
    //         cerr << "Shit happens!\n";
    // }
    // for (const auto& poly : input_ideal) {
    //     if (!ideal.contains(poly))
    //         cerr << "Shit happens!\n";
    // }
    // Poly x = Poly(Monomial{1});
    // Poly y = Poly(Monomial{0, 1});
    // Poly z = Poly(Monomial{0, 0, 1});
    // input_ideal.add(x * x + y);
    // input_ideal.add(y);

    // std::future<double> lex_res = std::async(std::launch::async, lex_test, input_ideal);
    // std::future<double> deglex_res = std::async(std::launch::async, deglex_test, input_ideal);
    // std::future<double> degrevlex_res = std::async(std::launch::async, degrevlex_test, input_ideal);
    // lex_res.get();
    // deglex_res.get();
    // degrevlex_res.get();

    std::thread lex_res(lex_test, input_ideal);
    std::thread deglex_res(deglex_test, input_ideal);
    std::thread degrevlex_res(degrevlex_test, input_ideal);
    lex_res.join();
    deglex_res.join();
    degrevlex_res.join();
    return 0;
}