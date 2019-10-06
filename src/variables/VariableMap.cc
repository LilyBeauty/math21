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
#include "VariableMap.h"

namespace math21 {
    namespace ad {

        // this will make previous references to pointer content invalid.
        // So use references to data.
        NumN VariableMap::createV(const char *name) {
            v.push(new Variable());
            Variable &x2 = *v.getLast();
            x2.setId(v.size());
            x2.setName(name);
            V.add(x2.getId());
            return x2.getId();
        }

        // create constant.
        NumN VariableMap::createC(NumR x, const char *name) {
            NumN k = createV(name);
            Variable &vk = at(k);
            vk.setType(variable_type_constant);
            vk.getValue().setSize(1);
            vk.getValue() = x;
            return k;
        }

        // constant X by variable X
        void setSizeCXUXByX(const Set &X, VariableMap &data) {
            NumN x = 0;
            for (NumN i = 1; i <= X.size(); ++i) {
                NumN type = data.at(X(i)).getType();
                if (type != variable_type_constant) {
                    x = X(i);
                    // x is used
                    if (!data.at(x).getValue().isEmpty()) {
                        break;
                    }
                }
            }
            if (x != 0) {
                for (NumN i = 1; i <= X.size(); ++i) {
                    NumN type = data.at(X(i)).getType();
                    if (type == variable_type_constant) {
                        Variable &vk = data.at(X(i));
                        if (vk.getValue().isScalar()) {
                            if (data.at(x).getValue().isScalar()) {
                                continue;
                            }
                            NumR c = vk.getValue()(1);
                            vk.getValue().setSize(data.at(x).getValue().shape());
                            vk.getValue() = c;
                        }
                    } else {
                        if (data.at(X(i)).getValue().isEmpty()) {
                            data.at(X(i)).getValue().setSize(data.at(x).getValue().shape());
                            data.at(X(i)).getValue() = 0;
                        }
                    }
                }
            }
        }

        // constant Y by X
        void setSizeYByX(const Set &X, const Set &Y, VariableMap &data) {
            const ArrayN &d = data.at(X(1)).getValue().shape();
            for (NumN i = 1; i <= Y.size(); ++i) {
                NumN y = Y(i);
                data.at(y).getValue().setSize(d);
            }
        }

        // set size of y by x
        void _setSizeyByx(NumN x, NumN y, VariableMap &data) {
            const ArrayN &d = data.at(x).getValue().shape();
            if (!data.at(y).getValue().isSameSize(d)) {
                data.at(y).getValue().setSize(d);
            }
        }

        // set size of y by x
        void _setSizeCyByx(NumN x, NumN y, VariableMap &data) {
            const ArrayN &d = data.at(x).getValue().shape();
            if (!data.at(y).getValue().isSameSize(d)) {
                Variable &vk = data.at(y);
                NumR c = 0;
                if (vk.getValue().isScalar()) {
                    c = vk.getValue()(1);
                }
                vk.getValue().setSize(d);
                vk.getValue() = c;
            }
        }

        // set size of y by x
        void setSizeyByx(NumN x, NumN y, VariableMap &data) {
            NumN type = data.at(y).getType();
            if (type == variable_type_constant) {
                _setSizeCyByx(x, y, data);
            } else{
                _setSizeyByx(x, y, data);
            }
        }
    }
}