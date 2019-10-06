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
        enum {
            variable_type_default = 1,
            variable_type_input,
            variable_type_output,
            variable_type_constant,
        };

        const char *math21_type2string_variable(NumN variable_type);

        struct Variable {
        private:
            std::string name;
            NumN id;
            NumN type;
            Function *f;
            Set X;
            Set Y;
            TenR v;

            void init() {
                id = 0;
                f = 0;
                type = variable_type_default;
            }

            void copy(const Variable &v) {
                printf("Variable copy, error!\n");
            }

        public:
            Variable() {
                init();
            }

            Variable(const Variable &v) {
                init();
                copy(v);
            }

            Variable(Function *f) {
                init();
                this->f = f->clone();
            }

            virtual ~Variable() {
                if (f) {
                    delete f;
                    f = 0;
                }
            }

            void addx(NumN x) {
                X.add(x);
            }

            void setX(const Set &X0) {
                X0.copyTo(X);
            }

            Set &getX() {
                return X;
            }

            const Set &getX() const {
                return X;
            }

            void addy(NumN y) {
                Y.add(y);
            }

            Set &getY() {
                return Y;
            }

            TenR &getValue() {
                return v;
            }

            const TenR &getValue() const {
                return v;
            }

            const Set &getY() const {
                return Y;
            }

            void setf(Function *f) {
                this->f = f->clone();
            }

            Function &getf() {
                return *f;
            }

            NumB hasf() const {
                if (f == 0) {
                    return 0;
                }
                return 1;
            }

            const Function &getf() const {
                if (f == 0) {
                    printf("f = 0");
                }
                return *f;
            }

            void setId(NumN id) {
                this->id = id;
            }

            NumN getId() const {
                return id;
            }

            const std::string &getName() const {
                return name;
            }

            void setName(const char *name = 0) {
                if (name) {
                    this->name = name;
                } else {
                    this->name = "";
                }
            }

            NumN getType() const {
                return type;
            }

            void setType(NumN type) {
                this->type = type;
            }

            void log(const char *name2 = 0) const;

            void log(std::ostream &io, const char *name2 = 0) const;

        };
    }
}