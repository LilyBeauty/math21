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

#include "distributions.h"
#include "ran.h"

namespace math21 {
    ////
    void _math21_random_uniform(NumZ &x, think::RandomEngine &ran, NumR a0, NumR b0) {
        MATH21_ASSERT(b0 >= a0);
        NumZ a, b;
        a = xjR2Z_upper(a0);
        b = xjR2Z_lower(b0);
        MATH21_ASSERT(b >= a, "there is no interger in [" << a0 << ", " << b0 << "]");
        NumN i = ran.draw_NumN();
        x = (NumZ) (i % (b - a + 1) + a);
    }

    void math21_random_uniform(NumZ &x, think::RandomEngine &ran, NumR a0, NumR b0) {
        _math21_random_uniform(x, ran, a0, b0);
    }

    void math21_random_uniform(NumN &x, think::RandomEngine &ran, NumR a0, NumR b0) {
        MATH21_ASSERT(b0 >= a0 && b0 > 0);
        NumZ x0;
        _math21_random_uniform(x0, ran, math21_number_not_less_than(a0, 0), b0);
        x = (NumN) x0;
    }

    void math21_random_uniform(NumR &x, think::RandomEngine &ran, NumR a, NumR b) {
        MATH21_ASSERT(b >= a);
        NumR i = ran.draw_0_1();
        x = i * (b - a) + a;
    }

    ////
    void RanUniform::draw(NumR &x) {
        math21_random_uniform(x, engine, a, b);
    }

    void RanUniform::draw(NumN &x) {
        math21_random_uniform(x, engine, a, b);
    }

    void RanUniform::draw(NumZ &x) {
        math21_random_uniform(x, engine, a, b);
    }

    void math21_random_draw(NumN &x, think::Random &ran) {
        ran.draw(x);
    }

    void math21_random_draw(NumZ &x, think::Random &ran) {
        ran.draw(x);
    }

    void math21_random_draw(NumR &x, think::Random &ran) {
        ran.draw(x);
    }

    NumR math21_pr_poisson_probability(NumN n, NumN lambda) {
        NumR value = xjexp(-(NumZ) lambda) * xjpow(lambda, n) / xjfactorial(n);
        return value;
    }

    // [k!/(n!*(n-k)!)]*(p^k * (1-p)^(n-k))
    //  n trials, p probability of success.
    //  k: the number of successes over the n trials.
    // n>0, 0<=k<=n
    NumR math21_pr_binomial(NumN n, NumR p, NumN k) {
        return xjfactorial(n)*xjpow(p, k)*xjpow(1-p, n-k)/(xjfactorial(k)*(xjfactorial(n-k)));
    }
}