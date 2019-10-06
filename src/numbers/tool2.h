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

#include "number.h"
#include <cmath>
#include <sstream>

namespace math21 {

    template<typename T>
    T m21_log(const T &a) { return log(a); }

    template<typename T>
    T m21_sqr(const T &a) { return a * a; }

    template<typename T>
    T m21_abs(const T &x) {
        if (x >= 0) {
            return x;
        } else {
            return -x;
        }
    }

    template<typename T, typename S>
    NumR m21_pow(const T &a, const S &b) { return pow(a, b); }

    template<class T>
    const T &m21_max(const T &a, const T &b) { return b > a ? (b) : (a); }

    inline float m21_max(const double &a, const float &b) { return b > a ? (b) : float(a); }

    inline float m21_max(const float &a, const double &b) { return b > a ? float(b) : (a); }

    template<class T>
    const T &m21_min(const T &a, const T &b) { return b < a ? (b) : (a); }

    inline float m21_min(const double &a, const float &b) { return b < a ? (b) : float(a); }

    inline float m21_min(const float &a, const double &b) { return b < a ? float(b) : (a); }

    template<class T>
    T m21_sign(const T &a, const T &b) { return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a); }

    inline float m21_sign(const float &a, const double &b) { return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a); }

    inline float m21_sign(const double &a, const float &b) {
        return (float) (b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a));
    }

    template<class T>
    void m21_swap(T &a, T &b) {
        T dum = a;
        a = b;
        b = dum;
    }

    template<typename T>
    std::string math21_string_to_string(const T &x) {
        std::ostringstream oss;
        oss << x;
        return oss.str();
    }

    NumR math21_string_to_NumR(const std::string &s);

    NumN math21_string_to_NumN(const std::string &s);

    NumZ math21_string_to_NumZ(const std::string &s);

    template<typename LogType>
    void math21_string_log_2_string(const LogType &A, std::string &s) {
        std::ostringstream oss;
        A.log(oss);
        s = oss.str();
    }


    NumB math21_point_isEqual(const NumR &x, const NumR &y, NumR epsilon = 0);

    NumB math21_point_isEqual(const NumN &x, const NumN &y, NumR epsilon = 0);

    NumB math21_point_isEqual(const NumZ &x, const NumZ &y, NumR epsilon = 0);
}