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

#include "ten.h"
#include "tenView.h"
#include "tenSub.h"

namespace math21 {

    // assign B to A
    template<typename T, typename S>
    void math21_operator_tensor_assign_elementwise_recursive(Tensor<T> &A, const Tensor<S> &B) {
        if (B.isEmpty()) {
            return;
        }
        MATH21_ASSERT(A.isSameSize(B.shape()), "tensor size doesn't match in assign");

        if (A.dims() == 1) {
            for (NumN i = 1; i <= A.dim(1); i++) {
                A(i) = (T) B(i);
            }
        } else {
            for (NumN i = 1; i <= A.dim(1); i++) {
                VecN x(A.dims());
                x = 0;
                x(1) = i;
                TensorSub<T> ts = A.shrinkSub(x);
                TensorView<S> tv = B.shrinkView(x);
                math21_operator_tensor_assign_elementwise_recursive(ts, tv);
            }
        }
    }

    void math21_tool_setSize(Seqce<TenR> &X, const VecN &shape);

    // v is vector.
    template<typename T>
    void math21_tool_vec_2_seqce(const Tensor<T> &v, Seqce<Tensor<T> > &w) {
        MATH21_ASSERT(v.dims() == 1);
        if (w.size()!=v.size()) {
            w.setSize(v.size());
        }
        VecN d(1);
        d = 1;
        math21_tool_setSize(w, d);
        for (NumN i = 1; i <= w.size(); i++) {
            TenR &x = w.at(i);
            x(1) = v(i);
        }
    }

    template<typename T>
    NumB math21_operator_seqce_isEqual(Seqce<Tensor<T> > &v, Seqce<Tensor<T> > &w) {
        MATH21_ASSERT(v.size() == w.size());
        for (NumN i = 1; i <= w.size(); i++) {
            if (!math21_operator_isEqual(v.at(i), w.at(i))) {
                return 0;
            }
        }
        return 1;
    }
}