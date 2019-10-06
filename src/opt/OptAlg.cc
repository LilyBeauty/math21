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

#include "OptAlg.h"

namespace math21 {

    //#######################################
    GoldenSectionSearch::GoldenSectionSearch(Function &f, const NumR &_a, const NumR &_b) : f(f), a(_a), b(_b) {
        tao = 0.618;
        tol = 0.001;
        time = 0;
        time_max = MATH21_TIME_MAX;
        c = a + (1 - tao) * (b - a);
        d = b - (1 - tao) * (b - a);
        fc = f.valueAt(c);
        fd = f.valueAt(d);
    }

    void GoldenSectionSearch::solve() {
        while (1) {
            if (fc < fd) {
//                a = a;
                b = d;
                d = c;
                c = a + (1 - tao) * (b - a);
                fd = fc;
                fc = f.valueAt(c);
            } else {
                a = c;
//                b = b;
                c = d;
                d = b - (1 - tao) * (b - a);
                fc = fd;
                fd = f.valueAt(d);
            }

            if (b - a < tol) {
                break;
            }
        }
    }

    //#####################################
    IntervalLocation::IntervalLocation(Function &f, const NumR _b1) : f(f) {
        b0 = 0;
        b1 = _b1;
        f0 = f.valueAt(b0);
        f1 = f.valueAt(b1);
        time = 0;
        time_max = 30;
    }

    void IntervalLocation::solve() {
        if (f1 > f0) {
            b2 = b1;
            return;
        }
        while (1){
            b2 = 2*b1;
            f2 = f.valueAt(b2);
            if(f2>=f1)return;
            b0 = b1;
            b1 = b2;
            f1 = f2;

            if (time >= time_max) {
                MATH21_ASSERT(0, "IntervalLocation fail");
                break;
            }
            time++;
//            m21log("interval location", time);
//            m21log("interval location", f2);
        }
    }

    void IntervalLocation::getInterval(NumR &a, NumR &b) {
        a = b0;
        b = b2;
    }
}