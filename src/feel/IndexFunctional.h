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
     * y=f(x)
     *
     * */

    // T is NumN, NumZ, or NumR.
    // this function is just a wrapper around tensor,
    // and can be viewed as an index-generalized tensor.
    template<typename T>
    class IndexFunctional : public think::Operator {
    private:
        VecZ index_offset; // index = tensor index + index offset
        VecN index_cur;
        Tensor <T> v;

        template<typename VecZType>
        T &valueAt(const VecZType &index) {
            MATH21_ASSERT(math21_operator_container_is_larger(index, index_offset),
                          "" << index.log("index") << "\n"
                             << index_offset.log("index_offset") << "\n")
#ifdef MATH21_FLAG_IS_PARALLEL
            VecN index_cur(index.size());
#endif
            math21_operator_container_subtract_to_C(index, index_offset, index_cur);
            return v(index_cur);
        }

        template<typename VecZType>
        const T &valueAt_const(const VecZType &index) const {
            MATH21_ASSERT(math21_operator_container_is_larger(index, index_offset),
                          "" << index.log("index") << "\n"
                             << index_offset.log("index_offset") << "\n")

            VecN index_cur(index.size());
            math21_operator_container_subtract_to_C(index, index_offset, index_cur);
            return v(index_cur);
        }

    public:
        IndexFunctional() {}

        //must be virtual.
        virtual ~IndexFunctional() {}

        IndexFunctional(const IndexFunctional<T> &f) {
            *this = f;
        }

        IndexFunctional<T> &operator=(const IndexFunctional<T> &f) {
            index_offset.copyFrom(f.index_offset);
            index_cur.copyFrom(f.index_cur);
            v.copyFrom(f.v);
            return *this;
        }

        void setSize(const VecN &d, const VecZ &start) {
            MATH21_ASSERT(d.size() == start.size())
            v.setSize(d);
            setStartIndex(start);
        }

        void setStartIndex(const VecZ &start) {
            MATH21_ASSERT(v.dims() == start.size())
            index_offset.setSize(start.size());
            index_cur.setSize(start.size());
            math21_operator_container_subtract_A_k(start, 1, index_offset);
        }

        void setSize(const VecN &d) {
            VecZ start(d.size());
            start = 1;
            setSize(d, start);
        }

        //from left to right, d1 is lowest.
        void setSize(NumN d1) {
            VecN d(1);
            d = d1;
            setSize(d);
        }

        //from left to right, d2 is lowest.
        void setSize(NumN d1, NumN d2) {
            VecN d(2);
            d = d1, d2;
            setSize(d);
        }

        //from left to right, d3 is lowest.
        void setSize(NumN d1, NumN d2, NumN d3) {
            VecN d(3);
            d = d1, d2, d3;
            setSize(d);
        }

        //from left to right, d3 is lowest.
        void setSize(NumN d1, NumN d2, NumN d3, NumN d4) {
            VecN d(4);
            d = d1, d2, d3, d4;
            setSize(d);
        }

        //from left to right, d1 is lowest.
        void setStartIndex(NumZ d1) {
            VecZ d(1);
            d = d1;
            setStartIndex(d);
        }

        //from left to right, d2 is lowest.
        void setStartIndex(NumZ d1, NumZ d2) {
            VecZ d(2);
            d = d1, d2;
            setStartIndex(d);
        }

        //from left to right, d3 is lowest.
        void setStartIndex(NumZ d1, NumZ d2, NumZ d3) {
            VecZ d(3);
            d = d1, d2, d3;
            setStartIndex(d);
        }

        //from left to right, d4 is lowest.
        void setStartIndex(NumZ d1, NumZ d2, NumZ d3, NumZ d4) {
            VecZ d(4);
            d = d1, d2, d3, d4;
            setStartIndex(d);
        }

        NumB isEmpty() const {
            return v.isEmpty();
        }

        virtual T &operator()(const VecZ &index) {
            return valueAt(index);
        }

        virtual T &operator()(const VecN &index) {
            return valueAt(index);
        }

        virtual const T &operator()(const VecZ &index) const {
            return valueAt_const(index);
        }

        virtual const T &operator()(const VecN &index) const {
            return valueAt_const(index);
        }

        template<template<typename> class Container, typename NumZType>
        T &operator()(const Container<NumZType> &index_a, NumZType j1) {
            VecZ index(index_a.size() + 1);
            math21_operator_container_set_partially(index_a, index, 0, 0, index_a.size());
            index(index.size()) = j1;
            return this->operator()(index);
        }

        virtual T &operator()(NumZ j1) {
            VecZ index(1);
            index = j1;
            return this->operator()(index);
        }

        virtual T &operator()(NumZ j1, NumZ j2) {
            VecZ index(2);
            index = j1, j2;
            return this->operator()(index);
        }

        virtual T &operator()(NumZ j1, NumZ j2, NumZ j3) {
            VecZ index(3);
            index = j1, j2, j3;
            return this->operator()(index);
        }

        virtual const T &operator()(NumZ j1) const {
            VecZ index(1);
            index = j1;
            return this->operator()(index);
        }

        virtual const T &operator()(NumZ j1, NumZ j2) const {
            VecZ index(2);
            index = j1, j2;
            return this->operator()(index);
        }

        virtual const T &operator()(NumZ j1, NumZ j2, NumZ j3) const {
            VecZ index(3);
            index = j1, j2, j3;
            return this->operator()(index);
        }

        NumN size() const {
            return v.size();
        }

        // Todo: make it private
        Tensor <T> &getTensor() {
            return v;
        }

        const Tensor <T> &getTensor() const {
            return v;
        }

        NumZ getTensor_index_offset(NumN i) const {
            return index_offset(i);
        }

        template<typename VecZType>
        void get_start_index(VecZType &start) const {
            MATH21_ASSERT(!isEmpty())
            if (start.isSameSize(index_offset.size()) == 0) {
                start.setSize(index_offset.size());
            }
            math21_operator_container_add_A_k(index_offset, 1, start);
        }

        template<typename VecZType>
        void get_end_index(VecZType &end) const {
            MATH21_ASSERT(!isEmpty())
            if (end.isSameSize(index_offset.size()) == 0) {
                end.setSize(index_offset.size());
            }
            math21_operator_container_addToC(index_offset, v.shape(), end);
        }

        template<typename VecZType>
        void getIndex(NumN i, VecZType &index) const {
            VecZType start(index_offset.size());
            get_start_index(start);
            math21_operator_number_index_1d_to_nd(index, i, v.shape(), start);
        }
    };
}