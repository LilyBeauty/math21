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

#include "inner.h"
#include "arithmetic.h"

namespace math21 {
    // B=A+k
    template<typename VecType1, typename VecType2>
    void math21_operator_container_add_A_k(const VecType1 &A, NumR k, VecType2 &B) {
        MATH21_ASSERT(A.size() == B.size(), "vector size doesn't match");
        NumN i;
        NumN n = A.size();
//#pragma omp parallel for
        for (i = 1; i <= n; ++i) {
            B(i) = A(i) + k;
        }
    }

    // B=A-k
    template<typename VecType1, typename VecType2>
    void math21_operator_container_subtract_A_k(const VecType1 &A, NumR k, VecType2 &B) {
        MATH21_ASSERT(A.size() == B.size(), "vector size doesn't match");
        NumN i;
        NumN n = A.size();
//#pragma omp parallel for
        for (i = 1; i <= n; ++i) {
            B(i) = A(i) - k;
        }
    }

    // A=A-k
    template<typename VecType>
    void math21_operator_container_subtract_A_k_to(VecType &A, NumR k) {
        math21_operator_container_subtract_A_k(A, k, A);
    }

    // B=k-A
    template<typename VecType1, typename VecType2>
    void math21_operator_container_subtract_k_A(NumR k, const VecType1 &A, VecType2 &B) {
        MATH21_ASSERT(A.size() == B.size(), "vector size doesn't match");
        NumN i;
        NumN n = A.size();
//#pragma omp parallel for
        for (i = 1; i <= n; ++i) {
            B(i) = k - A(i);
        }
    }

    // B=k-A
    template<typename VecType>
    void math21_operator_container_subtract_k_A_to(NumR k, VecType &A) {
        math21_operator_container_subtract_k_A(k, A, A);
    }

    // C=A-B
    template<typename VecType1, typename VecType2, typename VecType3>
    void math21_operator_container_subtract_to_C(const VecType1 &A, const VecType2 &B, VecType3 &C) {
        MATH21_ASSERT(A.size() == B.size(), "vector size doesn't match");
        MATH21_ASSERT(A.size() == C.size(), "vector size doesn't match");
        NumN i;
        NumN n = A.size();
//#pragma omp parallel for
        for (i = 1; i <= n; ++i) {
            C(i) = A(i) - B(i);
        }
        MATH21_ASSERT_CHECK_VALUE_TMP(math21_operator_container_isfinite(C))
    }

    // A=A-B
    template<typename VecType, typename VecType2>
    void math21_operator_container_subtract_to_A(VecType &A, const VecType2 &B) {
        math21_operator_container_subtract_to_C(A, B, A);
    }

    // B=A-B
    template<typename VecType, typename VecType2>
    void math21_operator_container_subtract_to_B(const VecType &A, VecType2 &B) {
        math21_operator_container_subtract_to_C(A, B, B);
    }

    // C=A/B
    template<typename VecType1, typename VecType2, typename VecType3>
    void math21_operator_container_divide_to_C(const VecType1 &A, const VecType2 &B, VecType3 &C) {
        MATH21_ASSERT(A.size() == B.size(), "vector size doesn't match");
        MATH21_ASSERT(A.size() == C.size(), "vector size doesn't match");
        NumN i;
        NumN n = A.size();
//#pragma omp parallel for
        for (i = 1; i <= n; ++i) {
            MATH21_ASSERT(xjabs(B(i)) > MATH21_EPS, "divide zero!" << B(i));
            C(i) = A(i) / B(i);
        }
        MATH21_ASSERT_CHECK_VALUE_TMP(math21_operator_container_isfinite(C))
    }

    // A=A/B
    template<typename VecType, typename VecType2>
    void math21_operator_container_divide_to_A(VecType &A, const VecType2 &B) {
        math21_operator_container_divide_to_C(A, B, A);
    }

    // B=A/B
    template<typename VecType, typename VecType2>
    void math21_operator_container_divide_to_B(const VecType &A, VecType2 &B) {
        math21_operator_container_divide_to_C(A, B, B);
    }

    // C=A+B
    template<typename VecType1, typename VecType2, typename VecType3>
    void math21_operator_container_addToC(const VecType1 &A, const VecType2 &B, VecType3 &C) {
        MATH21_ASSERT(A.size() == B.size(), "vector size doesn't match");
        MATH21_ASSERT(A.size() == C.size(), "vector size doesn't match");
        NumN i;
        NumN n = A.size();
//#pragma omp parallel for
        for (i = 1; i <= n; ++i) {
            C(i) = A(i) + B(i);
        }
        MATH21_ASSERT_CHECK_VALUE_TMP(math21_operator_container_isfinite(C))
    }

    // A=A+B
    template<typename VecType, typename VecType2>
    void math21_operator_container_addToA(VecType &A, const VecType2 &B) {
        math21_operator_container_addToC(A, B, A);
    }

    // B=A+B
    template<typename VecType, typename VecType2>
    void math21_operator_container_addToB(const VecType &A, VecType2 &B) {
        math21_operator_container_addToC(A, B, B);
    }

    // user should make sure that A(i) doesn't have type NumN when k<0.
    template<typename VecType>
    void math21_operator_container_letters(VecType &A, NumZ k = 1, NumN from = 0, NumN to = 0) {
        MATH21_ASSERT(!A.isEmpty());
        NumN i;
        NumN n = A.size();
        if (from == 0) {
            from = 1;
        }
        if (to == 0) {
            to = n;
        }
        MATH21_ASSERT(from >= 1 && from <= to && to <= n)
        for (i = from; i <= to; ++i) {
            A(i) = k;
            ++k;
        }
    }

    // user should make sure that A(i) doesn't have type NumN when k<0.
    template<typename VecType, typename T>
    void math21_operator_container_set_value(VecType &A, T k, T step, NumN from = 0, NumN to = 0) {
        MATH21_ASSERT(!A.isEmpty());
        NumN i;
        NumN n = A.size();
        if (from == 0) {
            from = 1;
        }
        if (to == 0) {
            to = n;
        }
        MATH21_ASSERT(from >= 1 && from <= to && to <= n)
        for (i = from; i <= to; ++i) {
            A(i) = k;
            k = k + step;
        }
    }

    // user should make sure that A(i) doesn't have type NumN when k<0.
    template<typename T, template<typename> class Container>
    void math21_operator_container_set_num(Container<T> &A, const T &k = 1, NumN from = 0, NumN to = 0) {
        MATH21_ASSERT(!A.isEmpty());
        NumN i;
        NumN n = A.size();
        if (from == 0) {
            from = 1;
        }
        if (to == 0) {
            to = n;
        }
        MATH21_ASSERT(from >= 1 && from <= to && to <= n)
        for (i = from; i <= to; ++i) {
            A(i) = k;
        }
    }

    // C=A*B
    template<typename VecType>
    void math21_operator_container_SchurProduct(const VecType &A, const VecType &B, VecType &C) {
        MATH21_ASSERT(A.size() == B.size(), "vector size doesn't match");
        MATH21_ASSERT(A.size() == C.size(), "vector size doesn't match");
        NumN i;
        NumN n = A.size();
//#pragma omp parallel for
        for (i = 1; i <= n; ++i) {
            C.at(i) = A(i) * B(i);
        }
    }

    // A=A*B
    template<typename VecType>
    void math21_operator_container_SchurProduct_to_A(VecType &A, const VecType &B) {
        math21_operator_container_SchurProduct(A, B, A);
    }

    // B=A*B
    template<typename VecType>
    void math21_operator_container_SchurProduct_to_B(const VecType &A, VecType &B) {
        math21_operator_container_SchurProduct(A, B, B);
    }

    // return first min value.
    template<typename T, template<typename> class Container>
    T math21_operator_container_min(const Container<T> &m) {
        NumN i;
        NumN n = m.size();
        MATH21_ASSERT(n >= 1);
        NumN k = 1;
        for (i = 2; i <= n; ++i) {
            if (m(i) < m(k)) {
                k = i;
            }
        }
        return m(k);
    }

    template<typename T, template<typename> class Container>
    T math21_operator_container_max(const Container<T> &m) {
        NumN i;
        NumN n = m.size();
        MATH21_ASSERT(n >= 1);
        NumN k = 1;
        for (i = 2; i <= n; ++i) {
            if (m(i) > m(k)) {
                k = i;
            }
        }
        return m(k);
    }

    template<typename VecType>
    NumN math21_operator_container_argmin(const VecType &m) {
        NumN i;
        NumN n = m.size();
        MATH21_ASSERT(n >= 1);
        NumN k = 1;
        for (i = 2; i <= n; ++i) {
            if (m(i) < m(k)) {
                k = i;
            }
        }
        return k;
    }


    template<typename T, template<typename> class Container>
    NumN math21_operator_container_index(const Container<T> &m, const T &x) {
        NumN n = m.size();
        for (NumN i = 1; i <= n; ++i) {
            if (m(i) == x) {
                return i;
            }
        }
        return 0;
    }

    template<typename T, template<typename> class Container>
    NumN math21_operator_container_arg(const Container<T> &m, const T &x) {
        return math21_operator_container_index(m, x);
    }

    // argmax from index k = 1
    template<typename VecType>
    NumN math21_operator_container_argmax(const VecType &m, NumN k = 1) {
        NumN i;
        NumN n = m.size();
        MATH21_ASSERT(k >= 1 && k <= n);
        for (i = k + 1; i <= n; ++i) {
            if (m(i) > m(k)) {
                k = i;
            }
        }
        return k;
    }

    //argmax every element of v.
    template<template<typename> class Container, typename VecType1, typename VecType2>
    void math21_operator_container_argmax(const Container<VecType1> &v, VecType2 &m) {
        MATH21_ASSERT(v.size() > 0, "v is empty");
        MATH21_ASSERT(m.size() == v.size());
        for (NumN i = 1; i <= v.size(); ++i) {
            m(i) = math21_operator_container_argmax(v(i));
        }
    }

    template<typename T, template<typename> class Container>
    T math21_operator_container_multiply_all(const Container<T> &x) {
        NumN n = x.size();
        MATH21_ASSERT(n >= 1);

        T sum = 1;
//#pragma omp parallel for
        for (NumN i = 1; i <= n; ++i) {
            sum = sum * x(i);
        }
        return sum;
    }

    template<typename VecType>
    NumB math21_operator_container_isEqual(const VecType &x, const VecType &y, NumR epsilon = 0) {
        if (x.size() == 0 || y.size() == 0) {
            if (x.size() != y.size()) {
                return 0;
            } else {
                return 1;
            }
        }
        NumN n = x.size();
        MATH21_ASSERT(n >= 1);
        MATH21_ASSERT(y.size() == n, "" << y.log("y") << x.log("x"));
        for (NumN i = 1; i <= n; ++i) {
            if (!math21_point_isEqual(x(i), y(i), epsilon)) {
                return 0;
            }
        }
        return 1;
    }

    template<typename T, typename VecType>
    NumB math21_operator_container_isEqual_c_array(const VecType &x, const T *y, NumR epsilon = 0) {
        NumN n = x.size();
        MATH21_ASSERT(n >= 1);
        NumR tmp;
        for (NumN i = 1; i <= n; ++i) {
            tmp = y[i - 1] - x(i);
            if (xjabs(tmp) > epsilon) {
                return 0;
            }
        }
        return 1;
    }

    template<typename VecType>
    NumB math21_operator_container_isEqual_num(const VecType &x, NumR k) {
        NumN n = x.size();
        MATH21_ASSERT(n >= 1);
        for (NumN i = 1; i <= n; ++i) {
            if (x(i) != k) {
                return 0;
            }
        }
        return 1;
    }

    template<typename VecType>
    NumB math21_operator_container_isEqualZero(const VecType &x) {
        return math21_operator_container_isEqual_num(x, 0);
    }

    template<typename VecType>
    NumR math21_operator_container_sum(const VecType &A, NumN n) {
        NumN i;
        NumR sum = 0;

        NumN size = A.size();
        if (n == 1) {
//#pragma omp parallel for
            for (i = 1; i <= size; ++i) sum += A(i);
        } else if (n == 2) {
//#pragma omp parallel for
            for (i = 1; i <= size; ++i) sum += xjsquare(A(i));
            sum = xjsqrt(sum);
        } else {
            MATH21_ASSERT(0, "norm other than 1, 2 not supported currently");
        }
        MATH21_ASSERT_FINITE(math21_operator_isfinite(sum))
        return sum;
    }

    template<typename VecType>
    NumR math21_operator_container_mean(const VecType &A) {
        MATH21_ASSERT(!A.isEmpty());
        NumR sum = math21_operator_container_sum(A, 1);
        return sum / A.size();
    }

    template<typename VecType>
    NumR math21_operator_container_norm(const VecType &A, NumN n) {
        NumN i;
        NumR sum = 0;
        NumN size = A.size();
        if (n == 1) {
//#pragma omp parallel for
            for (i = 1; i <= size; ++i) sum += xjabs(A(i));
        } else if (n == 2) {
//#pragma omp parallel for
            for (i = 1; i <= size; ++i) sum += xjsquare(A(i));
            sum = xjsqrt(sum);
        } else {
            MATH21_ASSERT(0, "norm other than 1, 2 not supported currently");
        }
        MATH21_ASSERT_FINITE(math21_operator_isfinite(sum))
        return sum;
    }

    template<typename VecType1, typename VecType2>
    NumR math21_operator_container_distance(const VecType1 &A, const VecType2 &B, NumN n) {
        MATH21_ASSERT(A.size() == B.size(), "vector size doesn't match");
        NumN i;
        NumR sum = 0;
        NumN size = A.size();
        if (n == 1) {
//#pragma omp parallel for
            for (i = 1; i <= size; ++i) sum += xjabs(A(i) - B(i));
        } else if (n == 2) {
//#pragma omp parallel for
            for (i = 1; i <= size; ++i) sum += xjsquare(A(i) - B(i));
            sum = xjsqrt(sum);
        } else {
            MATH21_ASSERT(0, "norm other than 1, 2 not supported currently");
        }
        MATH21_ASSERT_FINITE(math21_operator_isfinite(sum))
        return sum;
    }

    template<typename VecType>
    NumR math21_operator_container_InnerProduct(NumR k, const VecType &A, const VecType &B) {
        MATH21_ASSERT(!A.isEmpty(), "empty matrix");
        MATH21_ASSERT(A.size() == B.size());
        if (k == 0) {
            return 0;
        }
        NumN i;
        NumN n = A.size();
        NumR y = 0;
//#pragma omp parallel for
        for (i = 1; i <= n; ++i) {
            y = y + (A(i) * B(i));
        }
        y = y * k;
        MATH21_ASSERT_FINITE(math21_operator_isfinite(y))
        return y;
    }

    // y = k*x + b
    template<typename VecType, typename VecType2>
    void math21_operator_container_linear_kx_b(NumR k, const VecType &x, NumR b, VecType2 &y) {
        MATH21_ASSERT(!x.isEmpty(), "empty matrix");
        MATH21_ASSERT(x.size() == y.size());
        NumN i;
        NumN n = x.size();
//#pragma omp parallel for
        for (i = 1; i <= n; ++i) {
            y(i) = k * x(i) + b;
        }
    }

    // x <- k*x + b
    template<typename VecType>
    void math21_operator_container_linear_kx_b_to(NumR k, VecType &x, NumR b) {
        math21_operator_container_linear_kx_b(k, x, b, x);
    }

    // C = k1*A
    template<typename VecType, typename VecType2>
    void math21_operator_container_linear(NumR k1, const VecType &A, VecType2 &C) {
        MATH21_ASSERT(!A.isEmpty(), "empty matrix");
        MATH21_ASSERT(A.size() == C.size());
        NumN i;
        NumN n = A.size();
//#pragma omp parallel for
        for (i = 1; i <= n; ++i) {
            C(i) = (k1 * A(i));
        }
    }

    // A = k1*A
    template<typename VecType>
    void math21_operator_container_linear_to_A(NumR k1, VecType &A) {
        math21_operator_container_linear(k1, A, A);
    }

    // C = k1*A + k2*B
    template<typename VecType, typename VecType2, typename VecType3>
    void math21_operator_container_linear(NumR k1, const VecType &A, NumR k2, const VecType2 &B, VecType3 &C) {
        MATH21_ASSERT(!A.isEmpty(), "empty matrix");
        MATH21_ASSERT(A.size() == B.size());
        MATH21_ASSERT(A.size() == C.size());

        NumN i;
        NumN n = A.size();
//#pragma omp parallel for
        for (i = 1; i <= n; ++i) {
            C(i) = (k1 * A(i) + k2 * B(i));
        }
        MATH21_ASSERT_CHECK_VALUE_TMP(math21_operator_container_isfinite(C))
    }

    // A = k1*A + k2*B
    template<typename VecType, typename VecType2>
    void math21_operator_container_linear_to_A(NumR k1, VecType &A, const NumR k2, const VecType2 &B) {
        math21_operator_container_linear(k1, A, k2, B, A);
    }

    // B = k1*A + k2*B
    template<typename VecType, typename VecType2>
    void math21_operator_container_linear_to_B(NumR k1, const VecType &A, NumR k2, VecType2 &B) {
        math21_operator_container_linear(k1, A, k2, B, B);
    }

    // B=|A|
    template<typename VecType1, typename VecType2>
    void math21_operator_container_abs(const VecType1 &A, VecType2 &B) {
        MATH21_ASSERT(A.size() == B.size(), "vector size doesn't match");
        NumN i;
        NumN n = A.size();
//#pragma omp parallel for
        for (i = 1; i <= n; ++i) {
            B(i) = xjabs(A(i));
        }
    }

    // B=|A|
    template<typename VecType1>
    void math21_operator_container_abs_to(VecType1 &A) {
        math21_operator_container_abs(A, A);
    }

}