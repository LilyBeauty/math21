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

namespace math21 {
    namespace ad {
        void Function_sum::cr(const Set &X, NumN x, NumN dy, Set &output, VariableMap &data) const {
            Set input;
            NumN dx = data.createC(1, "1=d(sum(x))");
            if(isSetSize()){
                setSizeyByx(x, dx, data);
            }

            input.add(dx);
            input.add(dy);
            Function_multiply multiply;
            multiply.f(input, output, data);
            data.at(output(1)).setName("dx = d(sum(x)) * dy");
        }
    }
}