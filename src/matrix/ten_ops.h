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

#include "matrix.h"
#include "ten.h"

namespace math21 {

    // start from k.
    template<typename T>
    NumB math21_operator_isContinuousIntegers(const Tensor <T> &x, NumZ k = 1) {
        MATH21_ASSERT(!x.isEmpty());
        return math21_operator_container_isContinuousIntegers(x, k);
    }

    template<typename T>
    NumB math21_operator_isEqual(const Tensor <T> &x, const Tensor <T> &y, NumR epsilon = 0) {
        MATH21_ASSERT(!x.isEmpty());
        if (y.isSameSize(x.shape()) == 0) {
            return 0;
        }
        return math21_operator_container_isEqual(x, y, epsilon);
    }

    template<typename T>
    NumN math21_operator_number_of_equal(NumR k, const Tensor <T> &x) {
        MATH21_ASSERT(!x.isEmpty());
        return math21_operator_container_number_of_equal(k, x);
    }

    template<typename T>
    NumB math21_operator_is_not_less(const Tensor <T> &x, NumR k) {
        MATH21_ASSERT(!x.isEmpty());
        return math21_operator_container_is_not_less(x, k);
    }

    template<typename T>
    NumB math21_operator_is_less(const Tensor <T> &x, NumR k) {
        MATH21_ASSERT(!x.isEmpty());
        return math21_operator_container_is_less(x, k);
    }

    template<typename T>
    NumB math21_operator_is_not_larger(const Tensor <T> &x, NumR k) {
        MATH21_ASSERT(!x.isEmpty());
        return math21_operator_container_is_not_larger(x, k);
    }

    template<typename T>
    NumB math21_operator_is_larger(const Tensor <T> &x, NumR k) {
        MATH21_ASSERT(!x.isEmpty());
        return math21_operator_container_is_larger_number(x, k);
    }

    template<typename T>
    T math21_operator_multiply_all(const Tensor <T> &A) {
        MATH21_ASSERT(!A.isEmpty(), "empty matrix");
        return math21_operator_container_multiply_all(A);
    }

    // Todo: Deep question: Is it safe if v and w have a piece of common space? (next version)
    template<typename T>
    void math21_operator_shareReshape(const Tensor <T> &v, Tensor <T> &w, const VecN &d) {
        MATH21_ASSERT(v.volume() == math21_operator_multiply_all(d));
        SpaceParas paras = v.getSpace();
        w.setSize(d, &paras);
    }

    template<typename T>
    void math21_operator_shareReshape_to_vector(const Tensor <T> &v, Tensor <T> &w) {
        VecN d(1);
        d = v.volume();
        math21_operator_shareReshape(v, w, d);
    }

    template<typename T>
    void math21_operator_shareReshape_2d_to_3d(const Tensor <T> &v, Tensor <T> &w) {
        MATH21_ASSERT(v.dims() == 2)
        VecN d(3);
        d = 1, v.dim(1), v.dim(2);
        math21_operator_shareReshape(v, w, d);
    }

    template<typename T>
    void math21_operator_shareReshape(const Tensor <T> &v, Tensor <T> &w) {
        MATH21_ASSERT(v.volume() == w.volume());
        SpaceParas paras = v.getSpace();
        w.setSpace(paras);
    }

    // return shrink shape
    // e.x., b = 2, 1, 0.
    template<typename T>
    void math21_operator_tensor_shrink_shape(const Tensor <T> &A, const VecN &b, VecN &d) {
        NumN n = math21_operator_number_of_equal(0, b);
        if (n == 0) {
            d.clear();
            return;
        }
        d.setSize(n);
        for (NumN i = 1, j = 1; i <= b.size(); ++i) {
            if (b(i) == 0) {
                d(j) = A.dim(i);
                ++j;
            }
        }
    }

    // Has error!
    template<typename T>
    const T *math21_memory_tensor_data_address(const Tensor <T> &A) {
        MATH21_ASSERT(A.isContinuous())
        SpaceParas paras = A.getSpace();
        return (const T *) paras.start;
    }

    // Has error!
    template<typename T>
    T *math21_memory_tensor_data_address(Tensor <T> &A) {
        MATH21_ASSERT(A.isContinuous())
        SpaceParas paras = A.getSpace();
        return (T *) paras.start;
    }

    // not test
    template<typename T>
    void math21_memory_tensor_data_copy(Tensor <T> &A, const T *data) {
        MATH21_ASSERT(A.isContinuous())
        SpaceParas paras = A.getSpace();
        if (data != (const T *) paras.start) {
            math21_memory_memcpy(paras.start, data, sizeof(T) * A.size());
        }
    }
}