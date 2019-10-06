/* Copyright 2015 The math21 Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include <map>
#include <random>
#include "../algebra/files.h"
#include "inner.h"
#include "draw.h"

namespace math21 {

    void test_draw_random_normal_method1(NumB is_print) {
        RanNormal ran;

        NumN n = 200000;
        NumN nstars = 100;

        VecR m(n);
        ran.set(5, 2);
        math21_random_draw(m, ran);
        VecN p(10);
        p = 0;

        for (NumN i = 0; i < n; ++i) {
            double number = m(i + 1);
            if ((number >= 0.0) && (number < 10.0)) ++p(int(number) + 1);
        }

        if (is_print) {
            const char *name = "normal_distribution (5.0,2.0):";
            math21_pr_log_histogram_Y(p, name);
        }
    }

    void test_draw_random_normal_method2(NumB is_print) {
        const int nrolls = 200000;  // number of experiments
        const int nstars = 100;    // maximum number of stars to distribute

        std::default_random_engine generator;
        std::normal_distribution<double> distribution(5.0, 2.0);

        VecN p(10);
        p = 0;

        for (int i = 0; i < nrolls; ++i) {
            double number = distribution(generator);
            if ((number >= 0.0) && (number < 10.0)) ++p(int(number) + 1);
        }

        if (is_print) {
            const char *name = "normal_distribution (5.0,2.0):";
            math21_pr_log_histogram_Y(p, name);
        }
    }

    void test_time_random_normal() {
        NumN n = 1000;
        MATH21_PRINT_TIME_START()
        for (NumN i = 1; i <= n; ++i) {
            test_draw_random_normal_method1(0);
        }
        MATH21_PRINT_TIME_END("")
        MATH21_PRINT_TIME_START()
        for (NumN i = 1; i <= n; ++i) {
            test_draw_random_normal_method2(0);
        }
        MATH21_PRINT_TIME_END("")
    }

    void test_draw_random_uniform_method1(NumB is_print) {
        RanUniform ranUniform;
        ranUniform.set(0, 9);
        NumN n = 20000;
        VecN M(n);
        math21_random_draw(M, ranUniform);

        Dict<NumN, NumN> hist;
        for (NumN i = 1; i <= 20000; ++i) {
            if (hist.has(M(i))) {
                NumN &y = hist.valueAt(M(i));
                ++y;
            } else {
                hist.add(M(i), 1);
            }
        }
        if (is_print) {
            const char *name = "uniform distribution (0, 9):";
            VecN X, Y;
            hist.getX(X);
            hist.getY(Y);
            math21_pr_log_histogram_XY(X, Y, name);
        }
    }

    void test_draw_random_uniform_method2(NumB is_print) {
        std::random_device rd;
        std::uniform_int_distribution<int> dist(0, 9);

        Dict<NumN, NumN> hist;
        for (NumN i = 1; i <= 20000; ++i) {
            auto x = (NumN) dist(rd);
            if (hist.has(x)) {
                NumN &y = hist.valueAt(x);
                ++y;
            } else {
                hist.add(x, 1);
            }
        }
        if (is_print) {
            const char *name = "uniform distribution (0, 9):";
            VecN X, Y;
            hist.getX(X);
            hist.getY(Y);
            math21_pr_log_histogram_XY(X, Y, name);
        }
    }

    void test_time_random_uniform() {
        NumN n = 1000;
        MATH21_PRINT_TIME_START()
        for (NumN i = 1; i <= n; ++i) {
            test_draw_random_uniform_method1(0);
        }
        MATH21_PRINT_TIME_END("")
        MATH21_PRINT_TIME_START()
        for (NumN i = 1; i <= n; ++i) {
            test_draw_random_uniform_method2(0);
        }
        MATH21_PRINT_TIME_END("")
    }

    void test_draw_random_binomial(NumB is_print) {
        NumN size = 4;
        RanBinomial ran(size, 0.5);

        NumN n = 200000;
        VecN m(n);
        math21_random_draw(m, ran);
        VecN p(size + 1);
        p = 0;

        for (NumN i = 1; i <= m.size(); ++i) {
            NumN number = m(i);
            p(number + 1)++;
        }

        p.log("p");
        if (is_print) {
            const char *name = "binomial_distribution (9, 0.5):";
            math21_pr_log_histogram_Y(p, name);
        }
    }

    void test_draw_random_normal() {
        test_draw_random_normal_method1(1);
        test_draw_random_normal_method2(1);

//        test_time_random_normal();
    }

    void test_draw_random_uniform() {
        test_draw_random_uniform_method1(1);
        test_draw_random_uniform_method2(1);

//        test_time_random_uniform();
    }

    void test_draw_random_binomial() {
        test_draw_random_binomial(1);
    }
}