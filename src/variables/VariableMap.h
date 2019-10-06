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
    namespace ad {

        // VariableMap
        struct VariableMap {
        private:
            Seqce<Variable *> v;
            Set &V;

            void init() {
            }

        public:
            VariableMap(Set &V) : V(V) {
                init();
            }

            virtual ~VariableMap() {
                for (NumN i = 1; i <= size(); ++i) {
                    delete &at(i);
                }
            }

            NumN size() const {
                return v.size();
            }

            void log(const char *s = 0) {
                v.log(s);
            }

            // this will make previous references to pointer content invalid.
            // So use references to data.
            NumN createV(const char *name = 0);

            // create constant.
            NumN createC(NumR x, const char *name = 0);

            Variable &at(NumN i) {
                return *v.at(i);
            }

            const Variable &operator()(NumN i) const {
                return *v(i);
            }
        };

        // constant X by variable X
        void setSizeCXUXByX(const Set &X, VariableMap &data);

        // constant Y by X
        void setSizeYByX(const Set &X, const Set &Y, VariableMap &data);

        void setSizeyByx(NumN x, NumN y, VariableMap &data);
    }
}