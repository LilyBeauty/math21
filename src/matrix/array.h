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

#include <iostream>
#include <stdio.h>
#include <vector>
#include "inner.h"
#include "_array.h"

namespace math21 {

    // we must assure that element access is thread-safe.
    template<typename T>
    class Array {
    private:
        AutoBuffer autoBuffer;
        NumN n; // n is not auxiliary.
        T *v; // v is auxiliary.

        void init() {
            clear();
        }

        void updateAuxiliary() {
            v = (T *) autoBuffer.getObj();
        }

        void destroy() {
            MATH21_ASSERT_CODE((n > 0 && v != 0) || (n == 0 && v == 0),
                               "You've called setSizeNoSpace(), and should call setSpace() afterwards!");
            clear();
        }

    public:
        Array() {
            init();
        }

        Array(NumN n) {
            init();
            setSize(n);
        }

        // Copy constructor
        Array(const Array &B) {
//            MATH21_ASSERT(0, "Copy constructor.");
            B.copyTo(*this);
        }

        // Copy constructor
        template<typename S>
        Array(const Array<S> &B) {
            MATH21_ASSERT(0, "Copy constructor.");
            B.copyTo(*this);
        }

        virtual ~Array() {
            destroy();
        }

        NumB isEmpty() const {
            if (n == 0) {
                MATH21_ASSERT_CODE(v == 0);
                return 1;
            }
            MATH21_ASSERT_CODE(v != 0, "You've called setSizeNoSpace(), and should call setSpace() afterwards!");
            return 0;
        }

        // deprecate, because confict with Tensor as container.
        NumB isSameSize_zzz(NumN d) const {
            if (size() == d) {
                return 1;
            }
            return 0;
        }

        NumN size() const {
            return n;
        }

        //from left to right.
        NumN shape() const {
            return size();
        }

        // deprecate
        NumN nrows() const {
            return size();
        }

        NumN ncols() const {
            if (isEmpty()) {
                return 0;
            }
            return 1;
        }

        void ensureIndependence() {
            autoBuffer.ensureIndependence();
            updateAuxiliary();
        }

        void setSizeNoSpace(NumN n) {
            clear();
            if (n == 0) {
                return;
            }
//            autoBuffer.setSize(n * sizeof(T), paras);
            this->n = n;
            updateAuxiliary();
        }

        // keep size fixed, just set space.
        void setSpace(const SpaceParas *paras) {
            if (paras == 0) {
                return;
            }
            NumN n = size();
            setSize(n, paras);
        }

        // NOTE: data may not be zero
        void setSize(NumN n, const SpaceParas *paras = 0) {
            if (n == 0 && paras == 0) {
                clear();
                return;
            }
            autoBuffer.setSize(n * sizeof(T), paras);
            this->n = n;
            updateAuxiliary();
        }

        SpaceParas getSpace(NumN offset, NumN size, NumN unit = sizeof(char)) const {
            return autoBuffer.getSpace(offset, size, unit);
        }

        const T &operator()(NumN n, NumN m) const {
            MATH21_ASSERT_INDEX(n <= size() && m == 1);
            MATH21_ASSERT_CODE(v != 0,
                               "v is 0, maybe you forget call setSize, "
                               "or you've called setSizeNoSpace(), but didn't call setSpace() afterwards?")
            return v[n - 1];
        }

        const T &operator()(NumN n) const {
            MATH21_ASSERT_INDEX(n <= size());
            MATH21_ASSERT_CODE(v != 0,
                               "v is 0, maybe you forget call setSize, "
                               "or you've called setSizeNoSpace(), but didn't call setSpace() afterwards?")
            return v[n - 1];
        }

        T &operator()(NumN n, NumN m) {
            MATH21_ASSERT_INDEX(n <= size() && m == 1);
            MATH21_ASSERT_CODE(v != 0,
                               "v is 0, maybe you forget call setSize, "
                               "or you've called setSizeNoSpace(), but didn't call setSpace() afterwards?")
            return v[n - 1];
        }

        T &operator()(NumN n) {
            MATH21_ASSERT_INDEX(n <= size());
            MATH21_ASSERT_CODE(v != 0,
                               "v is 0, maybe you forget call setSize, "
                               "or you've called setSizeNoSpace(), but didn't call setSpace() afterwards?")
            return v[n - 1];
        }

        T &at(NumN n) {
            MATH21_ASSERT_INDEX(n <= size());
            MATH21_ASSERT_CODE(v != 0,
                               "v is 0, maybe you forget call setSize, "
                               "or you've called setSizeNoSpace(), but didn't call setSpace() afterwards?")
            return v[n - 1];
        }

        void clear() {
            v = 0;
            n = 0;
            autoBuffer.clear();
        }

        NumB log(const char *s = 0, NumB isLogDetail = 1) const {
            return log(std::cout, s, isLogDetail);
        }

        NumB log(std::ostream &io, const char *s = 0, NumB isLogDetail = 1) const {
            if (s) {
                io << "Array " << s << " (size " << size() << "):\n";
            }
            if (isLogDetail) {
                autoBuffer.log(io, s);
            }
            if (size() == 0) {
                io << "{}\n";
                return 0;
            }
            for (NumN i = 1; i <= size(); ++i) {
                if (i == 1) {
                    io << "{";
                }
                if (i != size()) {
                    io << (*this)(i) << ", ";
                } else {
                    io << (*this)(size()) << "}\n";
                }
            }
            return 1;
        }

        NumB isContinuous() const {
            return 1;
        }

        void copyTo(Array<T> &B) const {
            if (B.size() != size()) {
                B.setSize(size());
            }
            B.assign(*this);
        }

        //assignment
        Array<T> &operator=(const Array<T> &B) {
//            MATH21_ASSERT(0, "=");
            assign(B);
            return *this;
        }

        //assignment
        template<typename S>
        Array<T> &operator=(const Array<S> &B) {
            MATH21_ASSERT(0, "=");
//            assign(B);

            return *this;
        }

        // use carefully
        const AutoBuffer &getAutoBuffer() const {
            return autoBuffer;
        }

        void assign(const Array<T> &B) {
            MATH21_ASSERT(B.size() == size(),
                          "vector size doesn't match in assign");
            if (this != &B) {
                if (isContinuous() && B.isContinuous()) {
                    autoBuffer.setDataDeep(B.getAutoBuffer());
                    updateAuxiliary();
                } else {
                    NumN i;
                    for (i = 1; i <= size(); i++) (*this).at(i) = B(i);
                }
            }
        }

        void assign(const T &a) {
            NumN i;
            for (i = 1; i <= size(); i++) (*this).at(i) = a;
        }

        template<typename S>
        Array &operator=(
                const literal_assign_helper_container<S, Array> &val
        ) {
            MATH21_ASSERT(0, "literal_assign_helper")
//            val.getArray()->copyTo(*this);
            return *this;
        }

        template<typename S>
        const literal_assign_helper_container<T, Array> operator=(
                const S &val
        ) {
            // assign the given value to every spot in this matrix
            assign(val);
            // Now return the literal_assign_helper so that the user
            // can use the overloaded comma notation to initialize
            // the matrix if they want to.
            return literal_assign_helper_container<T, Array>(this);
        }

        void sort();

        template<typename Compare>
        void sort(const Compare &f);

        void swap(Array &B);
    };

    template<typename T>
    std::ostream &operator<<(std::ostream &out, const Array<T> &m) {
        m.log(out);
        return out;
    }

    typedef Array<NumN> ArrayN;
    typedef Array<NumZ> ArrayZ;
    typedef Array<NumR> ArrayR;
}