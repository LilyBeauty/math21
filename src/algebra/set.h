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

    template<typename T>
    struct _Set {
    private:
        Seqce<T> v;
    public:
        _Set() {
            clear();
        }

        virtual ~_Set() {
        }

        NumN size() const {
            return v.size();
        }

        NumB isEmpty() const {
            if (size() == 0) {
                return 1;
            }
            return 0;
        }

        const T &operator()(NumN j) const {
            return v.operator()(j);
        }

        void add(const T &j) {
            if (!contains(j)) {
                v.push(j);
            }
        }

        void add(const _Set<T> &S) {
            for (NumN i = 1; i <= S.size(); ++i) {
                add(S(i));
            }
        }

        template<template<typename> class Container>
        void add(const Container<T> &S) {
            for (NumN i = 1; i <= S.size(); ++i) {
                add(S(i));
            }
        }

        void copyToNox(_Set<T> &S, const T &x) const {
            S.clear();
            for (NumN i = 1; i <= size(); ++i) {
                const T &s = (*this)(i);
                if (s != x)
                    S.add(s);
            }
        }

        void copyTo(_Set<T> &S) const {
            S.clear();
            for (NumN i = 1; i <= size(); ++i) {
                S.add((*this)(i));
            }
        }

        T &at(NumN i) {
            return v.at(i);
        }

        NumB contains(const T &x) const {
            for (NumN i = 1; i <= size(); ++i) {
                if (v.operator()(i) == x) {
                    return 1;
                }
            }
            return 0;
        }

        void clear() {
            v.clear();
        }

        void log(const char *s = 0) const {
            log(std::cout, s);
        }

        void log(std::ostream &io, const char *s = 0) const {
            if (s == 0) {
                s = "";
            }
            io << "_Set " << s << ":\n";
            v.log(io, 0, 1);
        }

        void intersect(const _Set<T> &X, _Set<T> &Y) const {
            Y.clear();
            for (NumN i = 1; i <= size(); ++i) {
                if (X.contains((*this)(i))) {
                    Y.add((*this)(i));
                }
            }
        }

        void difference(const _Set<T> &X, _Set<T> &Y) const {
            Y.clear();
            for (NumN i = 1; i <= size(); ++i) {
                if (!X.contains((*this)(i))) {
                    Y.add((*this)(i));
                }
            }
        }

        void difference(const T &x, _Set<T> &Y) const {
            _Set<T> X;
            X.add(x);
            difference(X, Y);
        }

        void difference(const T &x) {
            _Set<T> Y;
            difference(x, Y);
            swap(Y);
        }

        void swap(_Set<T> &X) {
            v.swap(X.v);
        }

        void sort() {
            v.sort();
        }

        // max elements
        NumN max() const {
            MATH21_ASSERT(!isEmpty())
            Set B;
            Seqce<T> v2;
            v.copyTo(v2);
            v2.sort();
            return v2(v2.size());
        }

    };

    template<typename T>
    NumB math21_operator_set_isEqual(const _Set<T> &a, const _Set<T> &b) {
        if (a.size() != b.size()) {
            return 0;
        }
        Set c;
        a.intersect(b, c);
        if (c.size() == a.size()) {
            return 1;
        }
        return 0;
    }

    template<typename T>
    std::ostream &operator<<(std::ostream &out, const _Set<T> &m) {
        m.log(out);
        return out;
    }

    // argwhere from index k = 1
    template<template<typename> class Container, typename T>
    void math21_operator_container_argwhere(const Container<T> &m,
                                            const T &x, SetN &set, NumR epsilon = 0, NumN k = 1) {
        NumN i;
        NumN n = m.size();
        set.clear();
        MATH21_ASSERT(k >= 1 && k <= n);
        for (i = k; i <= n; ++i) {
            if (math21_point_isEqual(m(i), x, epsilon)) {
                set.add(i);
            }
        }
    }

}