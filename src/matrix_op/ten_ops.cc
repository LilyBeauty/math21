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

#include "inner.h"

namespace math21 {

    // C = k1*A
    void math21_operator_linear(NumR k1, const TenR &A, TenR &C) {
        MATH21_ASSERT(!A.isEmpty(), "empty matrix");

        if (C.isSameSize(A.shape()) == 0) {
            C.setSize(A.shape());
        }
        math21_operator_container_linear(k1, A, C);
    }

    // !! Note: A, B can be vectors, C may still be vector.
    // We don't require output C. So we write this vector version.
    // C = k1*A + k2*B
    // Now we can assure that if C is given right size beforehand, then C keeps the size. Otherwise, C will have type not fixed.
    void math21_operator_linear(NumR k1, const TenR &A, NumR k2, const TenR &B, TenR &C) {
        MATH21_ASSERT(!A.isEmpty(), "empty matrix");
        MATH21_ASSERT(A.isSameSizeVirtually(B.shape()),
                      "\t" << A.log("A") << "\n"
                           << "\t" << B.log("B") << "\n"
        );

        if (C.isSameSizeVirtually(A.shape()) == 0) {
            C.setSize(A.shape());
        }
        math21_operator_container_linear(k1, A, k2, B, C);
    }
}