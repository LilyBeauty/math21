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
    struct Interval {
    private:
        NumR a;
        NumR b;
        NumB include_a;
        NumB include_b;

        void init() {
            a = 0;
            b = 0;
            include_a = 1;
            include_b = 1;
        }

    public:

        void clear() {
            MATH21_ASSERT(a <= b, "" << log())

            a = 0;
            b = 0;
            include_a = 0;
            include_b = 0;
        }

        Interval() {
            init();
        }

        Interval(NumR a0, NumR b0, NumB inc_a, NumB inc_b) {
            set(a0, b0, inc_a, inc_b);
        }

        Interval(const Interval &interval) {
            *this = interval;
        }

        Interval &operator=(const Interval &interval) {
            a = interval.a;
            b = interval.b;
            include_a = interval.include_a;
            include_b = interval.include_b;
            return *this;
        }

        NumB isEqual(const Interval &interval) const {
            if (a == interval.a &&
                b == interval.b &&
                include_a == interval.include_a &&
                include_b == interval.include_b) {
                return 1;
            } else {
                return 0;
            }
        }

        void set(NumR a0, NumR b0, NumB inc_a, NumB inc_b) {
            a = a0;
            b = b0;
            include_a = inc_a;
            include_b = inc_b;
            if (a > b) {
                MATH21_ASSERT(a <= b, "" << log())
            }
        }

        void set(const Interval &I) {
            set(I.left(), I.right(), I.isLeftClosed(), I.isRightClosed());
        }

        virtual ~Interval() {
            clear();
        }

        NumR length() const {
            return b - a;
        }

        NumB log(const char *s = 0, NumB noEndl = 0) const {
            log(std::cout, s, noEndl);
            return 1;
        }

        // Todo: change NumB to void.
        NumB log(std::ostream &io, const char *s = 0, NumB noEndl = 0) const {
            if (s) {
                io << "Interval " << s << ":\n";
            }
            if (isLeftClosed()) {
                io << "[";
            } else {
                io << "(";
            }
            io << left() << ", " << right();
            if (isRightClosed()) {
                io << "]";
            } else {
                io << ")";
            }
            if (!noEndl) {
                io << std::endl;
            }
            return 1;
        }

        NumB isLeftClosed() const {
            return include_a;
        }

        NumB isRightClosed() const {
            return include_b;
        }

        NumR left() const {
            return a;
        }

        NumR right() const {
            return b;
        }

        NumB isInclude(NumR x) const {
            if (x > a && x < b) {
                return 1;
            } else if (x == a && isLeftClosed()) {
                return 1;
            } else if (x == b && isRightClosed()) {
                return 1;
            } else {
                return 0;
            }
        }
    };

    std::ostream &operator<<(std::ostream &out, const Interval &m);

    VecN &math21_operator_interval_sort(const Seqce <Interval> &intervals, VecN &idx);

    NumB math21_point_isEqual(const Interval &x, const Interval &y, NumR epsilon = 0);

    // C is the minimum interval which encloses A and B
    void math21_algebra_interval1d_union(const Interval &A, const Interval &B, Interval &C);

    struct Interval2D {
    private:
        Seqce <Interval> I;
    public:

        Interval2D() {
            I.setSize(2);
        }

        Interval2D(const Interval2D &interval2D) {
            *this = interval2D;
        }

        Interval2D &operator=(const Interval2D &interval2D) {
            I.assign(interval2D.I);
            return *this;
        }

        NumB isEqual(const Interval2D &interval2D) const {
            return I.isEqual(interval2D.I);
        }


        virtual ~Interval2D() {
        }

        Interval &at(NumN i) {
            MATH21_ASSERT(i >= 1 && i <= 2)
            return I.at(i);
        }

        Interval &operator()(NumN i) {
            at(i);
        }

        const Interval &operator()(NumN i) const {
            MATH21_ASSERT(i >= 1 && i <= 2)
            return I.operator()(i);
        }


        void log(const char *s = 0, NumB noEndl = 0) const {
            log(std::cout, s, noEndl);
        }

        void log(std::ostream &io, const char *s = 0, NumB noEndl = 0) const {
            if (s) {
                io << "Interval 2d " << s << ":\n";
            }
            (*this)(1).log(io, 0, 1);
            io << "x";
            (*this)(2).log(io, 0, 1);
            if (!noEndl) {
                io << std::endl;
            }
        }
    };

    NumB math21_point_isEqual(const Interval2D &x, const Interval2D &y, NumR epsilon = 0);

    // C is the minimum 2d interval which encloses A and B
    void math21_algebra_interval2d_union(const Interval2D &A, const Interval2D &B, Interval2D &C);

}