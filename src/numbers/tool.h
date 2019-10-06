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

#pragma once

#include <cmath>
#include "number.h"

namespace math21 {
    template<typename T>
    int xjisnan(T x) {
        if (std::isnan(x)) {
            return 1;
        } else {
            return 0;
        }
    }

    template<typename T>
    NumB xjisfinite(T x) {
        if (std::isfinite(x)) {
            return 1;
        } else {
            return 0;
        }
    }

    template<typename T>
    NumB xjisinf(T x) {
        if (std::isinf(x)) {
            return 1;
        } else {
            return 0;
        }
    }

    template<typename T>
    T xjabs(const T &x) {
        if (x >= 0) {
            return x;
        } else {
            return -x;
        }
    }

    template<typename T>
    T xjsquare(const T &x) {
        return x * x;
    }

    template<typename T>
    NumR xjatan(T x) {
        return atan(x);
    }

    template<typename T>
    NumR xjtan(T x) {
        return tan(x);
    }

    template<typename T>
    NumR xjexp(T x) {

        return exp(x);
    }

    template<typename T, typename S>
    NumR xjpow(const T &a, const S &b) { return pow(a, b); }

    NumN xjfactorial(NumN n);

    template<typename T>
    NumR xjlog(T x) {
        return log(x);
    }

    template<typename T>
    NumR xjsin(T x) {
        return sin(x);
    }

    template<typename T>
    NumR xjcos(T x) {
        return cos(x);
    }

    template<typename T>
    NumR xjsqrt(T x) {
        return sqrt(x);
    }

    template<typename T>
    NumR xjacos(T x) {
        return acos(x);
    }

    template<typename T, typename S>
    T xjmin(const T &x, const S &y) {
        return x < y ? x : y;
    }

    template<typename T, typename S>
    T xjmax(const T &x, const S &y) {
        return x > y ? x : y;
    }

    template<typename T>
    int xjradian2degree(T &x, T &y) {
        y = x * 180 / XJ_PI;
        return 1;
    }

    template<typename T>
    T xjradian2degree(const T &x) {
        return (T) (x * 180 / XJ_PI);
    }

    template<typename T>
    int xjdegree2radian(T &x, T &y) {
        y = x * XJ_PI / 180;
        return 1;
    }

    template<typename T>
    T xjdegree2radian(const T &x) {
        return (T) (x * XJ_PI / 180);
    }

    template<typename T>
    int xjmathInsideClose(T &x, T &a, T &b) {
        if (x < a || x > b)return 0;
        else return 1;
    }

    template<typename T>
    int xjmathInsideOpen(T &x, T &a, T &b) {
        if (x <= a || x >= b)return 0;
        else return 1;
    }

    template<typename T>
    int xjmathIs0to9(T &c) {
        if ((char) c < '0' || (char) c > '9')return 0;
        else return 1;
    }

// user should check c.
    template<typename T>
    int xjmathTo0to9(T &c) {
        return (char) c - '0';
    }


    NumB xjIsEven(const NumZ &x);

    // is x in [a, b]
    template<typename T1, typename T2, typename T3>
    NumB xjIsIn(const T1 &x, const T2 &a, const T3 &b) {
        if (x < a || x > b) {
            return 0;
        } else {
            return 1;
        }
    }

    template<typename T, typename S>
    S xjmathScale(T x, T a, T b, S a2, S b2) {
        MATH21_ASSERT(b - a != 0, "b-a==0");
        if (x < a) {
            return a2;
        }
        if (x > b) {
            return b2;
        }
        return (S) (a2 + (x - a) * (b2 - a2) / (b - a));
    }

    NumZ xjR2Z_upper(const NumR &x);

    NumZ xjR2Z_lower(const NumR &x);

    // error ratio >= 0, b-|b|*ratio <= a <= b + |b|*ratio
    NumB xjIsApproximatelyEqual(const NumR &a, const NumR &b, NumR ratio);

    template<typename T, typename S>
    T math21_number_not_less_than(const T &x, const S &y) {
        if (x < y) {
            return (T) y;
        }
        return x;
    }

    // round to n decimal places
    template<typename T>
    NumR xjround(const T &x, NumN n) {
        return round(x * pow(10, n)) / pow(10, n);
    }
}