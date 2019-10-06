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
        class Derivative {
        private:
            VariableMap &data;
        public:

            Derivative(VariableMap &data) : data(data) {
            }

            virtual ~Derivative() {
            }

            const Set &getX(const NumN &y) {
                const Variable &vy = data(y);
                const Set &X1 = vy.getX();
                return X1;
            }

            void getX(Set &X, const NumN &y, const Set &V) {
                const Variable &vy = data(y);
                const Set &X1 = vy.getX();
                X1.intersect(V, X);
            }

            void getY(const NumN &x, Set &Y, const Set &V0) {
                const Variable &vx = data(x);
                const Set &Y1 = vx.getY();
                Y1.intersect(V0, Y);
            }

            NumB pass(const Set &X, Set &V0, const Set &Y) {
                if (X.size() == 0) {
                    return 0;
                }

                NumB flag = 0;
                for (NumN i = 1; i <= X.size(); ++i) {
                    NumN x = X(i);
                    if (Y.contains(x)) {
                        V0.add(x);
                        if (flag == 0) {
                            flag = 1;
                        }
                    } else {
                        const Set &Y1 = data(x).getY();
                        if (pass(Y1, V0, Y)) {
                            V0.add(x);
                            if (flag == 0) {
                                flag = 1;
                            }
                        }
                    }
                }
                return flag;
            }

            // V0 = {V |X->v->Y}
            void restrictSet(const Set &V, Set &V0, const Set &X, const Set &Y) {
                V0.clear();
                pass(X, V0, Y);
//                V0.isEmpty()
            }

            // debug: n, level
            NumN cd(const NumN &x, Set &V, const Set &V0, Map &DT, NumN n=1, NumN level =1) {
                if (DT.has(x)) {
                    NumN y;
                    DT.get(x, y);
                    return y;
                }

                Set Y;
                getY(x, Y, V0);

                Set S;
                Set output;
                for (NumN j = 1; j <= Y.size(); ++j) {
                    NumN y = Y(j);
                    Function &f = data.at(y).getf();
                    NumN dy = cd(y, V, V0, DT, n, level+1);
                    Set X;
                    getX(X, y, V);
                    f.cr(X, x, dy, output, data);
                    NumN dxi = output(1);
                    S.add(dxi);
                }
                Function_sum sum;
                sum.f(S, output, data);
                NumN dx = output(1);
                data.at(dx).setName("dx=sum_dxi");
                DT.add(x, dx);
                return dx;
            }

            // compute derivative. Set value to constant. n is used for debug.
            void cds(const Set &X, NumN y, Set &V, Map &dX, NumN n=1) {
                Set V0;
                Set Y;
                Y.add(y);
                restrictSet(V, V0, X, Y);

                Map DT;
                NumN dy = data.createC(1, "dy");
                if(Function::isSetSize()){
                    setSizeyByx(y, dy, data);
                }
                DT.add(y, dy);
                NumN level = 1;
                for (NumN i = 1; i <= X.size(); ++i) {
                    cd(X(i), V, V0, DT, n, level);
                }
                DT.restrictTo(X, dX);

                Set X0;
                Set X1;
                dX.getX().toSet(X1);
                X.difference(X1, X0);
                if (!X0.isEmpty()) {
                    for (NumN i = 1; i <= X0.size(); ++i) {
                        NumN dx0 = data.createC(0, "0");
                        if(Function::isSetSize()){
                            setSizeyByx(X0(i), dx0, data);
                        }
                        dX.add(X0(i), dx0);
                    }
                }
            }

            void fv(const Set &X, NumN y, const Set &V) {
                if (X.contains(y)) {
                    return;
                }
                NumN type = data.at(y).getType();
                if (type == variable_type_constant) {
                    return;
                }
                const Set &X0 = getX(y);
                for (NumN i = 1; i <= X0.size(); ++i) {
                    NumN x = X0(i);
                    fv(X, x, V);
                }
                Function &f = data.at(y).getf();
                Set Y;
                Y.add(y);
                f.fv(data(y).getX(), Y, data);
            }

            // compute function values where no variable size in program level will be considered.
            void fvs(const Set &X, const Set &Y, const Set &V) {
                for (NumN i = 1; i <= Y.size(); ++i) {
                    fv(X, Y(i), V);
                }
            }

            void setSizesAllRelated(const Set &X, NumN y, const Set &V) {
                if (X.contains(y)) {
                    return;
                }
                NumN type = data.at(y).getType();
                if (type == variable_type_constant) {
                    return;
                }
                const Set &X0 = getX(y);
                for (NumN i = 1; i <= X0.size(); ++i) {
                    NumN x = X0(i);
                    setSizesAllRelated(X, x, V);
                }
                Function &f = data.at(y).getf();
                Set Y;
                Y.add(y);
                f.setSize(data(y).getX(), Y, data);
            }

            void setSizesAllRelated(const Set &X, Set &Y, const Set &V) {
                Y.sort();
                for (NumN i = 1; i <= Y.size(); ++i) {
                    setSizesAllRelated(X, Y(i), V);
                }
            }

            void setSizesXToY_zzz(const Set &X, const Set &Y, const Set &V) {
                Set V0;
                restrictSet(V, V0, X, Y);
                V0.sort();
                for (NumN i = 1; i <= V0.size(); ++i) {
                    NumN y = V0(i);
                    if (X.contains(y)) {
                        continue;
                    }
                    NumN type = data.at(y).getType();
                    if (type == variable_type_constant) {
                        continue;
                    }
                    Function &f = data.at(y).getf();
                    Set Y0;
                    Y0.add(y);
                    f.setSize(data(y).getX(), Y0, data);
                }
            }

            //call only once, set size to variables and adjust size of constant if necessary.
            // Y is ordered.
            void setSizes_zzz(const Set &X, Set &Y, const Set &V) {
//                setSizesXToY(X, Y, V);
//                setSizesAllRelated(X, Y, V);
            }
        };
    }
}