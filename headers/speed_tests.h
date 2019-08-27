#pragma once

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
#include "field.h"

#include <random>

namespace SpeedTest {
    using namespace std;

    using namespace SALIB;
    std::random_device rd;
    std::mt19937 mt(rd());

    template<typename Func>
    void get_all_combinations(int n, int k, Func callback, int last_used, vector<int> &result) {
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

    template<typename Func>
    void get_all_combinations(int n, int k, Func callback) {
        vector<int> res;
        get_all_combinations(n, k, callback, -1, res);
    }

    template <typename CoefficientType, typename Order>
    Polynomial<CoefficientType, Order> get_symmetric_k(int n, int k) {
        Polynomial<CoefficientType, Order> res;
        get_all_combinations(n, k, [&res](const vector<int> &comb) {
            Polynomial<CoefficientType, Order> poly_comb = CoefficientType(1);
            for (auto idx : comb) {
                poly_comb *= Monomial(idx);
            }
            res += poly_comb;
        });
        return res;
    }

    template <typename CoefficientType, typename Order>
    PolynomialSet<CoefficientType, Order> calc_cyclic_n_basis(int n) {
        using Poly = Polynomial<CoefficientType, Order>;
        using CurrentPolyAlg = PolyAlg<CoefficientType, Order>;
        using PolySet = PolynomialSet<CoefficientType, Order>;

        PolySet ideal;
        for (int idx = 1; idx < n; ++idx) {
            ideal.add(get_symmetric_k<CoefficientType, Order>(n, idx));
        }

        ideal.add(get_symmetric_k<CoefficientType, Order>(n, n) - Poly(CoefficientType((n % 2 == 0) ? -1 : 1)));

        CurrentPolyAlg algo;
        ideal = algo.auto_reduce(ideal);
        PolySet basis = algo.make_groebner_basis(ideal);
        basis = algo.auto_reduce(basis);
        if (basis.size() != n) {
            cerr << "BAD BASIS SIZE\n";
            exit(0);
        }
        return basis;
    }
}