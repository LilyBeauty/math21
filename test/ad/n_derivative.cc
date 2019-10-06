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

#include "n_derivative.h"

namespace math21 {
    namespace ad {
        // sin(x): x, sin(x), dy=1, dx
        void test_n_derivative_sin() {
            Set X;
            Set V;
            NumN y;
            VariableMap data(V);
            NumN x = data.createV("x");
            Variable &vx = data.at(x);
            vx.setType(variable_type_input);
            vx.getValue().setSize(2);
            vx.getValue() = 2, 3.14;
//            vx.getValue().setSize(1);
//            vx.getValue() = 3.14;
            Set input;
            input.add(x);
            X.add(input);

            Set output;
            Function_sin sin;
            sin.f(input, output, data);

            y = output(1);
            Variable &vy = data.at(y);
            vy.setType(variable_type_output);

            // set size
            Set Y;
            Y.add(y);

//            NumN n = 1;
            NumN n = 20;
            Derivative d(data);
            Map dX;
            for(NumN i=1; i<=n; ++i){
                d.cds(X, y, V, dX, i);
                y = dX.getY()(1);
                Y.add(y);
            }

            d.fvs(X, Y, V);

            X.log("X");
            Y.log("Y");
            V.log("V");
            dX.log("dX");
            data.log("data666");
        }


        // 2*sin(x): x, sin(x), dy=1, dx
        void test_n_derivative_2_sin() {
            Set X;
            Set V;
            NumN y;
            VariableMap data(V);
            NumN x = data.createV("x");
            Variable &vx = data.at(x);
            vx.setType(variable_type_input);
            vx.getValue().setSize(2);
            vx.getValue() = 2, 3.14;
//            vx.getValue().setSize(1);
//            vx.getValue() = 3.14;
            Set input;
            input.add(x);
            X.add(input);

            Set output;
            Function_sin sin;
            sin.f(input, output, data);

            output.copyTo(input);
            Function_multiply multiply;
            NumN k = data.createC(2, "2");
            if(Function::isSetSize()){
                setSizeyByx(input(1), k, data);
            }
            input.add(k);
            multiply.f(input, output, data);

            y = output(1);
            Variable &vy = data.at(y);
            vy.setType(variable_type_output);

            Set Y;
            Y.add(y);

//            NumN n = 1;
            NumN n = 20;
            Derivative d(data);
            Map dX;
            for(NumN i=1; i<=n; ++i){
                d.cds(X, y, V, dX, i);
                dX.get(x, y);
            }

            Y.add(y);
            d.fvs(X, Y, V);

            X.log("X");
            Y.log("Y");
            V.log("V");
            dX.log("dX");
            data.log("data");
        }


        // sin(cos(x)): x, sin(x), dy=1, dx
        void test_n_derivative_sin_cos() {
            Set X;
            Set V;
            NumN y;
            VariableMap data(V);
            NumN x = data.createV();
            Variable &vx = data.at(x);
            vx.getValue().setSize(3);
            vx.getValue() =1, 2, 3.14;
            Set input;
            input.add(x);
            X.add(input);

            Set output;
            Function_cos cos;
            cos.f(input, output, data);
            output.copyTo(input);

            Function_sin sin;
            sin.f(input, output, data);
            y = output(1);


//            NumN n = 1;
//            NumN n = 3;
            NumN n = 7;
            Derivative d(data);
            Map dX;
            for(NumN i=1; i<=n; ++i){
                d.cds(X, y, V, dX, i);
                dX.get(x, y);
            }

            Set Y;
            Y.add(y);
            d.fvs(X, Y, V);

            X.log("X");
            Y.log("Y");
            data(y).log("dx");
//            V.log("V");
            dX.log("dX");
//            data.log("data");
        }


        // 2*cos(sin(cos(x))): x, sin(x), dy=1, dx
        void test_n_derivative_2_cos_sin_cos() {
            Set X;
            Set V;
            NumN y;
            VariableMap data(V);
            NumN x = data.createV();
            Variable &vx = data.at(x);
//            vx.getValue().setSize(1);
//            vx.getValue() = 3.14;
            vx.getValue().setSize(2);
            vx.getValue() = 2, 3.14;
            Set input;
            input.add(x);
            X.add(input);

            Set output;
            Function_cos cos;
            cos.f(input, output, data);
            output.copyTo(input);

            Function_sin sin;
            sin.f(input, output, data);
            output.copyTo(input);

            cos.f(input, output, data);
            output.copyTo(input);

            Function_multiply multiply;
            NumN k = data.createC(2, "2");
            if(Function::isSetSize()){
                setSizeyByx(input(1), k, data);
            }
            input.add(k);
            multiply.f(input, output, data);
            y = output(1);

//            NumN n = 1;
//            NumN n = 3;
            NumN n = 7;
            Derivative d(data);
            Map dX;
            for(NumN i=1; i<=n; ++i){
                d.cds(X, y, V, dX, i);
                dX.get(x, y);
            }

            Set Y;
            Y.add(y);
            d.fvs(X, Y, V);

            data(y).log("dx");
//            X.log("X");
//            Y.log("Y");
//            V.log("V");
//            dX.log("dX");
//            data.log("data");
        }


        void test_n_derivative_all() {
            test_n_derivative_sin();
//            test_n_derivative_2_sin();
//            test_n_derivative_sin_cos();
//            test_n_derivative_2_cos_sin_cos();
        }
    }
}