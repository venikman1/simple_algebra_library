//#include <iostream>
//#include <cassert>
//#include <string>
//#include <boost/rational.hpp>
//#include <vector>
//
//#include "polynomial.h"
//#include "polynomial_set.h"
//#include "algebra_io.h"
//#include "tests.h"
//#include "algorithms.h"
//#include "speed_tests.h"
//
//#include <random>
//
//using std::cout;
//using std::cin;
//using std::cerr;
//
//using namespace SALIB;
//std::random_device rd;
//std::mt19937 mt(rd());
//
//using Rat = boost::rational<long long>;
//using Poly = Polynomial<Rat>;
//using DefPoly = Polynomial<Rat>;
//using PolySet = PolynomialSet<Rat>;
//using DefPolySet = PolynomialSet<Rat>;
//
//Monomial random_monomial(size_t var_limit, int count = 10) {
//    Monomial res;
//    for (int i = 0; i < count; ++i) {
//        ++res[mt() % var_limit];
//    }
//    return res;
//}
//
//DefPoly random_polynomial(size_t var_limit = 4, int monomial_count = 3, int count = 3) {
//    DefPoly res;
//    for (int i = 0; i < monomial_count; ++i) {
//        res += DefPoly(random_monomial(var_limit, count), Rat(mt() % 5));
//    }
//    return res;
//}
//
//void do_calculation(int count) {
//    // CustomOrder grlex(
//    //     &GradientSemiOrder::cmp,
//    //     &MonoLexOrder::cmp
//    // );
//    DefaultOrder grlex;
//
//    PolyAlg<Rat, DefaultOrder> algo;
//    DefPoly one = Poly(Rat(1));
//    long long full_ideals = 0;
//    for (int i = 0; i < count; ++i) {
//        DefPolySet ideal;
//        for (int j = 0; j < 5; ++j) {
//            ideal.add(random_polynomial());
//        }
//        if (algo.is_polynomial_in_ideal(one, ideal, grlex)) {
//            ++full_ideals;
//        } else {
//            DefPoly p;
//            for (const auto& pd : ideal) {
//                p += random_polynomial() * pd;
//            }
//            assert(algo.is_polynomial_in_ideal(p, ideal, grlex) == true);
//            assert(algo.is_polynomial_in_ideal(p + one, ideal, grlex) == false);
//        }
//    }
//    cerr << count << " ideals were generated\n";
//    cerr << full_ideals << " ideals contained a constant\n";
//}
//
//void use_all_pc_power() {
//    do_calculation(1);
//}