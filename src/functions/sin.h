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
        struct Function_sin : public Function {
        private:
        public:
            Function_sin() {
            }

            virtual ~Function_sin() {
            }

            void cr(const Set &X, NumN x, NumN dy, Set &output, VariableMap &data) const;

            void f(const Set &X, Set &output, VariableMap &data) {
                NumN x = X(1);
                NumN y = data.createV("sin(x)");
                if(isSetSize()){
                    setSizeyByx(x, y, data);
                }
                data.at(y).setf(this);

                data.at(y).addx(x);
                data.at(x).addy(y);
                output.clear();
                output.add(y);
            }

            void fv(const Set &X, const Set &Y, VariableMap &data) const {
                NumN y = Y(1);
                math21_operator_sin(data(X(1)).getValue(), data.at(y).getValue());
            }

            void setSize(const Set &X, const Set &Y, VariableMap &data) const {
                setSizeYByX(X, Y, data);
            }


            Function *clone() const {
                Function *f = new Function_sin();
                return f;
            }

            const char *getName() const {
                return "sin";
            }
        };
    }
}