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

#include "tool.h"

namespace math21 {
    NumB xjIsEven(const NumZ &x) {
        if (xjabs(x) % 2 == 0) {
            return 1;
        } else {
            return 0;
        }
    }

    NumZ xjR2Z_upper(const NumR &x) {
        NumZ y = (NumZ) x;
        if (y < x) {
            y = y + 1;
        }
        return y;
    }

    NumZ xjR2Z_lower(const NumR &x) {
        NumZ y = (NumZ) x;
        if (y > x) {
            y = y - 1;
        }
        return y;
    }

    // ratio >= 0
    NumB xjIsApproximatelyEqual(const NumR &a, const NumR &b, NumR ratio) {
        NumR c = xjabs(b) * ratio;
        if (a >= b - c && a <= b + c) {
            return 1;
        } else {
            return 0;
        }
    }

    NumN xjfactorial(NumN n) {
        if (n == 0) {
            return 1;
        }
        NumN i;
        NumN y = 1;
        for (i = 1; i <= n; ++i) {
            y = y * i;
        }
        return y;
    }
}