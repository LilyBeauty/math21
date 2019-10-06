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

#include "Variable.h"

namespace math21 {
    namespace ad {
        const char *math21_type2string_variable(NumN variable_type) {
#define MATH21_LOCAL_F(a) case a: return MATH21_STRINGIFY(a);
            switch (variable_type) {
                MATH21_LOCAL_F(variable_type_default)
                MATH21_LOCAL_F(variable_type_input)
                MATH21_LOCAL_F(variable_type_output)
                MATH21_LOCAL_F(variable_type_constant)
                default:
                    return "UNKNOWN";
            }
#undef MATH21_LOCAL_F
        }

        void Variable::log(const char *name2) const {
            log(std::cout, name2);
        }

        void Variable::log(std::ostream &io, const char *name2) const {
            io << "id: " << id << ", ";
            io << "name: " << (name2 == 0 ? name.c_str() : name2) << ", ";
            io << "type: " << math21_type2string_variable(type) << "\n";
            io << "f name: " << (f == 0 ? "" : f->getName()) << ", ";
            io << "f addr: " << f << "\n";
            v.log(io, "value");
        }
    }
}