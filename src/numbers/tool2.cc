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

#include "files.h"
#include "tool2.h"

namespace math21 {
    NumR math21_string_to_NumR(const std::string &s) {
        std::istringstream iss(s);
        NumR num;
        iss >> num;
        return num;
    }

    NumN math21_string_to_NumN(const std::string &s) {
        std::istringstream iss(s);
        NumN num;
        iss >> num;
        return num;
    }

    NumZ math21_string_to_NumZ(const std::string &s) {
        std::istringstream iss(s);
        NumZ num;
        iss >> num;
        return num;
    }

    NumB math21_point_isEqual(const NumR &x, const NumR &y, NumR epsilon) {
        NumR tmp;
        tmp = y - x;
        if (xjabs(tmp) > epsilon) {
            return 0;
        } else {
            return 1;
        }
    }

    NumB math21_point_isEqual(const NumN &x, const NumN &y, NumR epsilon) {
        return math21_point_isEqual((NumR) x, (NumR) y, epsilon);
    }

    NumB math21_point_isEqual(const NumZ &x, const NumZ &y, NumR epsilon) {
        return math21_point_isEqual((NumR) x, (NumR) y, epsilon);
    }
}