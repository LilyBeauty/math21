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
#include "sequence.h"

namespace math21 {

    template<typename T, typename S>
    struct _Map {
    private:
        _Sequence <T> a;
        _Sequence <S> b;

        void init();

    public:

        _Map();

        virtual ~_Map();

        NumN size() const;

        NumB isEmpty() const;

        const _Sequence <T> &getX() const;

        const _Sequence <S> &getY() const;

        void get(const T &x, S &y);

        S &valueAt(const T &x);

        // get key at position i.
        // position is virtual.
        const T &keyAtIndex(NumN i) const;

        const S &valueAtIndex(NumN i) const;


        NumN has(const T &x);

        void add(const T &x, const S &y);

        void add(const _Set <T> &X, const S &y);

        void clear();

        void restrictTo(const _Set <T> &X, _Map<T, S> &dst);

        void log(const char *s = 0) const;

        void log(std::ostream &io, const char *s = 0) const;
    };

    template<typename T, typename S>
    void _Map<T, S>::init() {
        clear();
    }

    template<typename T, typename S>
    _Map<T, S>::_Map() {
        init();
    }

    template<typename T, typename S>
    _Map<T, S>::~_Map() {
        clear();
    }

    template<typename T, typename S>
    NumN _Map<T, S>::size() const {
        return a.size();
    }

    template<typename T, typename S>
    NumB _Map<T, S>::isEmpty() const {
        if (size() == 0) {
            return 1;
        } else {
            return 0;
        }
    }

    template<typename T, typename S>
    const _Sequence <T> &_Map<T, S>::getX() const {
        return a;
    }

    template<typename T, typename S>
    const _Sequence <S> &_Map<T, S>::getY() const {
        return b;
    }

    template<typename T, typename S>
    void _Map<T, S>::get(const T &x, S &y) {
        for (NumN i = 1; i <= a.size(); ++i) {
            if (a(i) == x) {
                y = b.at(i);
                return;
            }
        }
        MATH21_ASSERT(0)
    }

    template<typename T, typename S>
    S &_Map<T, S>::valueAt(const T &x) {
        for (NumN i = 1; i <= a.size(); ++i) {
            if (a(i) == x) {
                return b.at(i);
            }
        }
        MATH21_ASSERT(0)
    }

    // get key at position i.
    // position is virtual.
    template<typename T, typename S>
    const T &_Map<T, S>::keyAtIndex(NumN i) const {
        return a(i);
    }

    template<typename T, typename S>
    const S &_Map<T, S>::valueAtIndex(NumN i) const {
        return b(i);
    }


    template<typename T, typename S>
    NumN _Map<T, S>::has(const T &x) {
        for (NumN i = 1; i <= a.size(); ++i) {
            if (a(i) == x) {
                return i;
            }
        }
        return 0;
    }

    template<typename T, typename S>
    void _Map<T, S>::add(const T &x, const S &y) {
        a.add(x);
        b.add(y);
    }

    template<typename T, typename S>
    void _Map<T, S>::add(const _Set <T> &X, const S &y) {
        for (NumN i = 1; i <= X.size(); ++i) {
            add(X(i), y);
        }
    }

    template<typename T, typename S>
    void _Map<T, S>::clear() {
        a.clear();
        b.clear();
    }

    template<typename T, typename S>
    void _Map<T, S>::restrictTo(const _Set <T> &X, _Map<T, S> &dst) {
        dst.clear();
        for (NumN i = 1; i <= size(); ++i) {
            if (X.contains(a(i))) {
                dst.add(a(i), b(i));
            }
        }
    }

    template<typename T, typename S>
    void _Map<T, S>::log(const char *s) const {
        log(std::cout, s);
    }

    template<typename T, typename S>
    void _Map<T, S>::log(std::ostream &io, const char *s) const {
        if (s == 0) {
            s = "";
        }
        io << "_Map " << s << ":\n";
        for (NumN i = 1; i <= size(); ++i) {
            io << "(" << a(i) << ", " << b(i) << ")\n";
        }
    }

}