#include <iostream>
#include <cassert>
#include <string>
#include <boost/rational.hpp>
#include <vector>
#include <boost/multiprecision/gmp.hpp>

#include "polynomial.h"
#include "polynomial_set.h"
#include "algebra_io.h"
#include "tests.h"
#include "algorithms.h"
#include "speed_tests.h"
#include "stopwatch.h"
#include "orders.h"

#include <random>

using namespace std;

using namespace SALIB;
std::random_device rd;
std::mt19937 mt(rd());

//using Rat = boost::rational<long long>;
using Rat = boost::multiprecision::mpq_rational;
using Poly = Polynomial<Rat>;
using PolySet = PolynomialSet<Rat>;
using PolyVec = vector<Poly>;

template <typename Func>
void get_all_combinations(int n, int k, Func callback, int last_used, vector<int>& result) {
    int current_size = result.size();
    int need_get = k - current_size;
    int can_get = n - 1 - last_used;
    if (need_get > can_get)
        return;
    if (need_get == 0) {
        callback(static_cast<const vector<int> &>(result));
        return;
    }
    for (int idx = last_used + 1; idx < n; ++idx) {
        result.push_back(idx);
        get_all_combinations(n, k, callback, idx, result);
        result.pop_back();
    }
}

template <typename Func>
void get_all_combinations(int n, int k, Func callback) {
    vector<int> res;
    get_all_combinations(n, k, callback, -1, res);
}

Poly get_symmetric_k(int n, int k) {
    Poly res;
    get_all_combinations(n, k, [&res](const vector<int> & comb) {
        Poly poly_comb = Rat(1);
        for (auto idx : comb) {
            poly_comb *= Monomial(idx);
        }
        res += poly_comb;
    });
    return res;
}

void calc_cyclic_n(int n) {
//    using PolyAlgTemp = PolyAlg<Rat>;

    using DegRevLex = CustomOrder<MonoGradientSemiOrder, RevOrder<MonoLexOrder>>; // Change order here
//    using DegRevLex = MonoLexOrder;
    using PolyAlgTemp = PolyAlg<Rat, DegRevLex>;
    using DegRevLexPolySet = PolynomialSet<Rat, DegRevLex>;

    PolySet ideal;
    for (int idx = 1; idx < n; ++idx) {
        ideal.add(get_symmetric_k(n, idx));
    }
    ideal.add(get_symmetric_k(n, n) - Poly(Rat(n % 2 == 0 ? -1 : 1)));

    PolyAlgTemp algo;
    DegRevLexPolySet basis = algo.make_groebner_basis(ideal);
    basis = algo.auto_reduce(basis);
    cout << "Groebner basis:\n";
    for (const auto& poly : basis) {
        cout << " > " << poly << "\n";
    }
}