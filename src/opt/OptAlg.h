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
    /*
     * min f(x)
     *
     * */
    class GoldenSectionSearch : public think::Optimization {
    private:
        Function &f;
        NumR a, b, c, d;
        NumR tao, tol;
        NumR fc, fd;
        NumN time, time_max;
    public:
        GoldenSectionSearch(Function &f, const NumR &_a, const NumR &_b);

        virtual ~GoldenSectionSearch() {}

        void solve();

        NumR getMinimum() { return c; }
    };

    /*
     * find interval location of argmin along positive x axis.
     * */
    class IntervalLocation : public think::Optimization {
    private:
        Function &f;
        NumR b0, b1, b2;
        NumR f0,f1,f2;
        NumN time, time_max;
    public:
        IntervalLocation(Function &f, const NumR _b1=0.001);

        virtual ~IntervalLocation() {}

        void solve();

        void getInterval(NumR &a, NumR &b);
    };
}