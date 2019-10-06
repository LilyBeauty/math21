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
        struct Function_multiply : public Function {
        private:
        public:
            Function_multiply() {
            }

            virtual ~Function_multiply() {
            }

            void cr(const Set &X, NumN x, NumN dy, Set &output, VariableMap &data) const {
                Set X1;
                X.copyToNox(X1, x);
                Set input;

                Function_multiply multiply;
                if(X1.isEmpty()){
                    NumN dx = data.createC(1, "1=d(multiply(x))");
                    if(isSetSize()){
                        setSizeyByx(x, dx, data);
                    }
                    output.clear();
                    output.add(dx);
                } else{
                    multiply.f(X1, output, data);
                    data.at(output(1)).setName("d(multiply(x))");
                }

                output.copyTo(input);
                input.add(dy);
                multiply.f(input, output, data);
                data.at(output(1)).setName("dx = d(multiply(x)) * dy");
            }

            void f(const Set &X, Set &output, VariableMap &data) {
                NumN y = data.createV("multiply(x)");
                if(isSetSize()){
                    setSizeyByx(X(1), y, data);
                }
                data.at(y).setf(this);
                data.at(y).setX(X);
                for (NumN i = 1; i <= X.size(); ++i) {
                    data.at(X(i)).addy(y);
                }
                output.clear();
                output.add(y);
            }

            // dot product
            void fv(const Set &X, const Set &Y, VariableMap &data) const {
                NumN y = Y(1);
                data.at(y).getValue().assign(data.at(X(1)).getValue());
                for (NumN i = 2; i <= X.size(); ++i) {
                    math21_operator_SchurProduct_to_B(data(X(i)).getValue(), data.at(y).getValue());
                }
            }

            void setSize(const Set &X, const Set &Y, VariableMap &data) const {
                setSizeCXUXByX(X, data);
                setSizeYByX(X, Y, data);
            }


            Function *clone() const {
                Function *f = new Function_multiply();
                return f;
            }

            const char *getName() const {
                return "multiply";
            }

        };
    }
}