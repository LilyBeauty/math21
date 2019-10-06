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

#include "operations.h"
#include "mat_ops.h"

namespace math21 {

    void math21_operator_mat_eye(MatR &A) {
        if (A.isEmpty()) {
            return;
        }
        A = 0;
        NumN r = m21_min(A.nrows(), A.ncols());
        NumN i;
        for (i = 1; i <= r; i++) A(i, i) = 1;
    }


#ifndef MATH21_FLAG_USE_CUDA

    void math21_cuda_test_02() {}

#endif

}