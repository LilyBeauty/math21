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

namespace math21 {
    /*
     * RULE: all tenser ops must obey the following rules:
     * If tensor A as input and output at the same time, then A will keep its type.
     * #If tensor A as output only, then A is the smallest type, i.e., if A can be Matrix and vector, then A must be vector.
     * Not good, some bugs maybe. Will consider later.
     *
     * Currently, I think caller should call set size. This will create type. All tensor ops just calculate, not doing set size jobs.
     * But, it will increase code. So set size step is put here temporally.
     *
     * If output tensor A is given right size beforehand, then its type and size are fixed.
     * So if we want to make sure the output tensor is
     * actually a vector, we can set the right size before call the method.
     * */

    // When a number is bad, it will be useless. So it should be clipped from every matrix using it.

    template<typename T, typename S, template<typename> class Container>
    void math21_operator_setData_by_container(Tensor <T> &A, const Container<S> &x) {
        MATH21_ASSERT(A.volume() == x.size());
        math21_operator_container_set(x, A);
    }

    template<typename T, typename S, template<typename> class Container>
    void math21_operator_setData_to_container(const Tensor <T> &A, Container<S> &x) {
        MATH21_ASSERT(A.volume() == x.size());
        math21_operator_container_set(A, x);
    }

    template<typename T>
    void math21_operator_sort(Tensor <T> &A) {
        MATH21_ASSERT(!A.isEmpty(), "empty matrix");
        math21_algorithm_sort(A);
    }

    template<typename T, typename Compare>
    void math21_operator_sort(Tensor <T> &A, const Compare &f) {
        MATH21_ASSERT(!A.isEmpty(), "empty matrix");
        math21_algorithm_sort(A, f);
    }

    template<typename T>
    void math21_operator_clip_not_less_than_eps(Tensor <T> &A) {
        MATH21_ASSERT(!A.isEmpty(), "empty matrix");
        math21_operator_container_clip_not_less_than_eps(A);
    }


    template<typename T>
    NumB math21_clip(Tensor <T> &A) {
        return math21_clip_container(A);
    }

    template<typename T>
    NumB math21_check_clip(const Tensor <T> &A) {
        return math21_check_clip_container(A);
    }

    template<typename T>
    NumB math21_operator_isEqualZero(const Tensor <T> &A) {
        return math21_operator_container_isEqualZero(A);
    }

////////////////////////// math
    template<typename T>
    T math21_operator_max(const Tensor <T> &m) {
        MATH21_ASSERT(!m.isEmpty());
        return math21_operator_container_max(m);
    }

    template<typename T>
    T math21_operator_min(const Tensor <T> &m) {
        MATH21_ASSERT(!m.isEmpty());
        return math21_operator_container_min(m);
    }

    template<typename T>
    NumN math21_operator_argmax(const Tensor <T> &m) {
        MATH21_ASSERT(!m.isEmpty());
        return math21_operator_container_argmax(m);
    }

    template<typename T>
    NumN math21_operator_argmin(const Tensor <T> &m) {
        MATH21_ASSERT(!m.isEmpty());
        return math21_operator_container_argmin(m);
    }


    template<typename T>
    void math21_operator_add(NumR k, const Tensor <T> &x, TenR &y) {
        MATH21_ASSERT(x.isEmpty() == 0);
        MATH21_ASSERT(y.isSameSize(x.shape()));
//        if (y.isSameSize(x.shape()) == 0) {
//            y.setSize(x.shape());
//        }
        math21_operator_container_add(k, x, y);
    }

    template<typename T>
    void math21_operator_addTo(NumR k, Tensor <T> &x) {
        MATH21_ASSERT(x.isEmpty() == 0);
        math21_operator_container_addTo(k, x);
    }

    template<typename T>
    void math21_operator_divide(NumR k, const Tensor <T> &x, TenR &y) {
        MATH21_ASSERT(x.isEmpty() == 0);
        MATH21_ASSERT(y.isSameSize(x.shape()));
        math21_operator_container_divide(k, x, y);
    }

    template<typename T>
    void math21_operator_divide_to(NumR k, Tensor <T> &x) {
        MATH21_ASSERT(x.isEmpty() == 0);
        math21_operator_container_divide_to(k, x);
    }

    template<typename T>
    void math21_operator_sqrt(const Tensor <T> &X, TenR &Y) {
        if (X.isEmpty()) {
            return;
        }
        if (Y.isSameSize(X.shape()) == 0) {
            Y.setSize(X.shape());
        }
        MATH21_ASSERT(Y.isSameSize(X.shape()), "tensor size doesn't match");
        math21_operator_container_sqrt(X, Y);
    }

    template<typename T>
    void math21_operator_sqrt_to(Tensor <T> &X) {
        MATH21_ASSERT(!X.isEmpty());
        math21_operator_container_sqrt_to(X);
    }

//////////////

// assign log(X) to Y, Y must have same with X.
    template<typename T>
    void math21_operator_log(const Tensor <T> &X, TenR &Y) {
        if (X.isEmpty()) {
            return;
        }
        if (Y.isSameSize(X.shape()) == 0) {
            Y.setSize(X.shape());
        }
        MATH21_ASSERT(Y.isSameSize(X.shape()), "tensor size doesn't match");
        math21_operator_container_log(X, Y);
    }

// assign exp(X) to Y, Y must have same with X.
    template<typename S>
    void math21_operator_exp(const Tensor <S> &X, TenR &Y) {
        if (X.isEmpty()) {
            return;
        }
        if (Y.isSameSize(X.shape()) == 0) {
            Y.setSize(X.shape());
        }
        MATH21_ASSERT(Y.isSameSize(X.shape()), "tensor size doesn't match");
        math21_operator_container_exp(X, Y);
    }

//scale x in [a,b] to [c, d]
    template<typename S, typename T>
    void math21_operator_scale(const Tensor <S> &X, Tensor <T> &Y, NumR a, NumR b, NumR c, NumR d) {
        if (X.isEmpty()) {
            return;
        }
        MATH21_ASSERT(Y.isSameSize(X.shape()), "tensor size doesn't match");
        MATH21_ASSERT(b > a && d > c, "scale range negative");

        ArrayN index;
        index.setSize(X.dims());
        index = 1;
        NumR val;
        while (1) {
            val = (X(index) - a) * (d - c) / (b - a) + c;
            Y(index) = (T) val;
            if (math21_operator_container_increaseNumFromRight(X.shape(), index) == 0) {
                break;
            }
        }
    }

//scale x in [a,b] to [c, d]
    template<typename S>
    void math21_operator_scale(Tensor <S> &X, NumR a, NumR b, NumR c, NumR d) {
        math21_operator_scale(X, X, a, b, c, d);
    }

// Y can be X1, X2.
    template<typename S1, typename S2, typename T>
    void
    _math21_test_operator_container_increaseNumFromRight(const Tensor <S1> &X1, const Tensor <S2> &X2, Tensor <T> &Y) {
        if (X1.isEmpty() || X2.isEmpty()) {
            return;
        }
        MATH21_ASSERT(Y.isSameSize(X1.shape()), "tensor size doesn't match");
        MATH21_ASSERT(Y.isSameSize(X2.shape()), "tensor size doesn't match");
        VecN d;
        d.setSize(X1.dims());
        d = 1;
        while (1) {
            Y(d) = X1(d) - X2(d);
            if (math21_operator_container_increaseNumFromRight(X1.shape(), d) == 0) {
                break;
            }
        }
    }

// Y can be X1, X2.
    template<typename T>
    NumR math21_operator_InnerProduct(NumR k, const Tensor <T> &A, const Tensor <T> &B) {
        MATH21_ASSERT(!A.isEmpty());
        MATH21_ASSERT(A.isSameSize(B.shape()), "tensor size doesn't match");
        return math21_operator_container_InnerProduct(k, A, B);
    }

//return max, index will be argmax.
    template<typename T>
    T math21_operator_tensor_max_zzz(const Tensor <T> &X, ArrayN &index) {
        MATH21_ASSERT(!X.isEmpty(), "max a empty vector");
        T sum = (T) 0;
        ArrayN d;
        d.setSize(X.dims());
        d = 1;
        T max = (T) MATH21_MIN;
        if (index.isEmpty()) {
            index.setSize(d.size());
        }
        MATH21_ASSERT(index.size() == d.size());

        index = 1;
        while (1) {
            const T &val = X(d);
            if (val > max) {
                max = val;
                index.assign(d);
            }
            if (math21_operator_container_increaseNumFromRight(X.shape(), d) == 0) {
                break;
            }
        }
        return max;
    }

    // assign B to A
    template<typename T, typename S>
    void math21_operator_tensor_assign_elementwise_no_recursive(Tensor <T> &A, const Tensor <S> &B) {
        if (B.isEmpty()) {
            return;
        }
        MATH21_ASSERT(A.isSameSize(B.shape()), "tensor size doesn't match in assign");
        VecN d;
        d.setSize(A.dims());
        d = 1;
        while (1) {
            A(d) = B(d);
            if (math21_operator_container_increaseNumFromRight(A.shape(), d) == 0) {
                break;
            }
        }
    }

    //assign X to v.
    template<typename T, typename VecType>
    void math21_operator_tensor_to_container_right(const Tensor <T> &X, VecType &v) {
        MATH21_ASSERT(!X.isEmpty(), "convert a empty tensor");
        if (v.isEmpty()) {
            v.setSize(X.volume());
        }
        MATH21_ASSERT(v.size() == X.volume());
        VecN d;
        d.setSize(X.dims());
        d = 1;
        NumN i = 1;
        while (1) {
            v(i) = X(d);
            ++i;
            if (math21_operator_container_increaseNumFromRight(X.shape(), d) == 0) {
                break;
            }
        }
    }

    //assign X to v.
    template<typename T, typename VecType>
    void math21_operator_tensor_to_container_right_in_batch(const Seqce <Tensor<T>> &u, Seqce <VecType> &v) {
        MATH21_ASSERT(!u.isEmpty(), "convert empty data");
        if (v.isEmpty()) {
            v.setSize(u.size());
        }
        MATH21_ASSERT(u.size() == v.size());
        for (NumN i = 1; i <= u.size(); ++i) {
            math21_operator_tensor_to_container_right(u(i), v.at(i));
        }
    }

/////////////////////////////////

    // C=A+B
    template<typename T>
    void math21_operator_add(const Tensor <T> &A, const Tensor <T> &B, Tensor <T> &C) {
        MATH21_ASSERT(A.isSameSize(B.shape()), "tensor size doesn't match");
        MATH21_ASSERT(A.isSameSize(C.shape()), "tensor size doesn't match");
        math21_operator_container_addToC(A, B, C);
    }

    template<typename T>
    void math21_operator_addToA(Tensor <T> &A, const Tensor <T> &B) {
        MATH21_ASSERT(A.isSameSize(B.shape()), "tensor size doesn't match");
        math21_operator_container_addToA(A, B);
//        math21_operator_container_addToA(A, B);
    }

    template<typename T>
    void math21_operator_addToB(const Tensor <T> &A, Tensor <T> &B) {
        MATH21_ASSERT(A.isSameSize(B.shape()), "tensor size doesn't match");
        math21_operator_container_addToB(A, B);
    }

    template<typename T>
    void math21_operator_SchurProduct(const Tensor <T> &A, const Tensor <T> &B, Tensor <T> &C) {
        MATH21_ASSERT(A.isSameSize(B.shape()), "tensor size doesn't match");
        MATH21_ASSERT(A.isSameSize(C.shape()), "tensor size doesn't match");
        math21_operator_container_SchurProduct(A, B, C);
    }

    template<typename T>
    void math21_operator_SchurProduct_to_A(Tensor <T> &A, const Tensor <T> &B) {
        MATH21_ASSERT(A.isSameSize(B.shape()), "\ttensor size doesn't match"
                << "\n\tA.shape(): (" << A.shape() << ")"
                << "\n\tB.shape(): (" << B.shape() << ")"
        );
        math21_operator_container_SchurProduct_to_A(A, B);
    }

    template<typename T>
    void math21_operator_SchurProduct_to_B(const Tensor <T> &A, Tensor <T> &B) {
        MATH21_ASSERT(A.isSameSize(B.shape()), "\ttensor size doesn't match"
                << "\n\tA.shape(): (" << A.shape() << ")"
                << "\n\tB.shape(): (" << B.shape() << ")"
        );
        math21_operator_container_SchurProduct_to_B(A, B);
    }

    template<typename T>
    void math21_operator_sin(const Tensor <T> &A, Tensor <T> &B) {
        MATH21_ASSERT(A.isSameSize(B.shape()), "tensor size doesn't match");
        math21_operator_container_sin(A, B);
    }

    template<typename T>
    void math21_operator_cos(const Tensor <T> &A, Tensor <T> &B) {
        MATH21_ASSERT(A.isSameSize(B.shape()), "tensor size doesn't match");
        math21_operator_container_cos(A, B);
    }

    template<typename T>
    void math21_operator_cosTo(Tensor <T> &A) {
        math21_operator_container_cosTo(A);
    }

/////////////////////////////////

    template<typename T>
    NumR math21_operator_norm(const Tensor <T> &A, NumN n) {
        return math21_operator_container_norm(A, n);
    }

    template<typename T>
    NumR math21_operator_sum(const Tensor <T> &A, NumN n) {
        return math21_operator_container_sum(A, n);
    }

/////////////////////////////////

// C = k1*A
    void math21_operator_linear(NumR k1, const TenR &A, TenR &C);

// C = k1*A + k2*B
    void math21_operator_linear(NumR k1, const TenR &A, NumR k2, const TenR &B, TenR &C);

/////////////////////////////////

    template<typename T, typename S,
            template<typename, typename> class MapType>
    void math21_operator_map_1d_to_tensor(const MapType<T, S> &f, TenR &A) {
        MATH21_ASSERT(!f.isEmpty())
        if (!A.isSameSize(f.size(), 2)) {
            A.setSize(f.size(), 2);
        }
        NumN n = f.size();
        for (NumN i = 1; i <= n; ++i) {
            A(i, 1) = f.keyAtIndex(i);
            A(i, 2) = f.valueAtIndex(i);
        }
    }

    // all maps have same size.
    template<typename T, typename S,
            template<typename, typename> class MapType,
            template<typename> class Container>
    void math21_operator_container_map_1d_to_tensor(const Container<MapType<T, S>> &v, TenR &A) {
        MATH21_ASSERT(!v.isEmpty())
        if (!A.isSameSize(v.size(), v(1).size(), 2)) {
            A.setSize(v.size(), v(1).size(), 2);
        }
        NumN n1 = v.size();
        NumN n2 = v(1).size();
        for (NumN i1 = 1; i1 <= n1; ++i1) {
            const MapType<T, S> &f = v(i1);
            for (NumN i2 = 1; i2 <= n2; ++i2) {
                A(i1, i2, 1) = f.keyAtIndex(i2);
                A(i1, i2, 2) = f.valueAtIndex(i2);
            }
        }
    }

    // assign B to A
    template<typename T, typename S>
    void math21_operator_tensor_assign_elementwise(Tensor <T> &A, const Tensor <S> &B) {
//        math21_operator_tensor_assign_elementwise_recursive(A, B);
        math21_operator_tensor_assign_elementwise_no_recursive(A, B);
    }
}