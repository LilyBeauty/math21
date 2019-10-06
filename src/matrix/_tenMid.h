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

#include "array.h"
#include "vec_ops.h"
#include "_tenBefore.h"

namespace math21 {

    template<typename T>
    class TensorView;

    template<typename T>
    class TensorSub;

    template<typename T>
    class Tensor {
    private:
        std::string name;
        Array<T> data;
        // Note: a is auxiliary, not necessary.
        Seqce<ArrayN> a; ////a is index to vector data. we can design a to be empty.
        ArrayN d;
//        Tensor<NumN> d2;
        // Note: N is auxiliary, not necessary.
        NumN N; //N = d.size()
        NumB is_column_major; // can be inferred from a.

        void _setSize_shape(const ArrayN &_d, NumB isClear = 1) {
            if (isClear) {
                if (!isEmpty()) {
                    clearSome();
                }
            }
            if (math21_operator_container_min(_d) == 0) {
                return;
            }
            _d.copyTo(d);
//            d2.setSize(d.size());
            N = d.size();
            MATH21_ASSERT(N > 0, "x-D tensor, and x must be positive integer");
        }

        // may deprecate
        NumB _setSize_check_data(NumN scale) {
            if (data.size() != scale) {
                return 0;
            }
            return 1;
        }

        //set size from left to right, from top to bottom.
        void _setSize_index(NumN &scale) {
            a.setSize(N);
            scale = 0;
            if (!isColumnMajor()) {
                for (NumN n = N; n >= 1; --n) {
                    a.at(n).setSize(dim(n));
                    if (n == N) {
                        for (NumN j = 1; j <= dim(n); j++) {
                            a.at(n).at(j) = j;
                        }
                        scale = dim(n);
                    } else {
                        for (NumN j = 1; j <= dim(n); j++) {
                            a.at(n).at(j) = (j - 1) * scale;
                        }
                        scale = scale * dim(n);
                    }
                }
            } else {
                for (NumN n = 1; n <= N; ++n) {
                    a.at(n).setSize(dim(n));
                    if (n == 1) {
                        for (NumN j = 1; j <= dim(n); j++) {
                            a.at(n).at(j) = j;
                        }
                        scale = dim(n);
                    } else {
                        for (NumN j = 1; j <= dim(n); j++) {
                            a.at(n).at(j) = (j - 1) * scale;
                        }
                        scale = scale * dim(n);
                    }
                }
            }
        }

        //set size from left to right, from top to bottom.
        void _setSize(const ArrayN &_d, const SpaceParas *paras = 0) {
            _setSize_shape(_d);
            if (isEmpty()) {
                return;
            }

            NumN scale = 0;
            _setSize_index(scale);
            data.setSize(scale, paras);
        }

        // space is null.
        void _setSizeNoSpace(const ArrayN &_d) {
            _setSize_shape(_d);
            if (isEmpty()) {
                return;
            }

            NumN scale = 0;
            _setSize_index(scale);
            data.setSizeNoSpace(scale);
        }

        void _setSizeDataFixed(const ArrayN &_d) {
            MATH21_ASSERT(volume() == math21_operator_container_multiply_all(_d), "data size check failed!");
            if (isEmpty()) {
                return;
            }
            _setSize_shape(_d, 0);

            NumN scale = 0;
            _setSize_index(scale);
        }

        NumB isLogAutoBuffer() const {
            return 0;
        }

        void clearSome() {
            name = "";
            data.clear();
            a.clear();
            d.clear();
            N = 0;
        }

        const Array<T> &getData() const {
            MATH21_ASSERT(isContinuous())
            return data;
        }

        // yes, don't use outside because the data may be stored
        // in arbitrary order, not just in row-major or column-major.
        Array<T> &getData() {
            MATH21_ASSERT(isContinuous())
            return data;
        }

        // deprecate
        NumN index_tensor_to_array(const Array<NumN> &index) const {
            MATH21_ASSERT(dims() == index.size(), "not " << dims() << "-D tensor index");
            NumN sum = 0;
            for (NumN i = 1; i <= dims(); i++) {
                MATH21_ASSERT(index(i) >= 1 && index(i) <= dim(i),
                              "\tYou must give a valid index"
                                      << "\n\tcurrent index: " << index(i)
                                      << "\n\trequired [" << 1 << ", " << dim(i) << "]");
                sum = sum + a(i)(index(i));
            }
            return sum;
        }

        NumN index_tensor_to_array(const Tensor<NumN> &index) const {
            MATH21_ASSERT(dims() == index.size(), "not " << dims() << "-D tensor index");
            NumN sum = 0;
            for (NumN i = 1; i <= dims(); i++) {
                MATH21_ASSERT(index(i) >= 1 && index(i) <= dim(i),
                              "\tYou must give a valid index"
                                      << "\n\tcurrent index: " << index(i)
                                      << "\n\trequired [" << 1 << ", " << dim(i) << "]");
                sum = sum + a(i)(index(i));
            }
            return sum;
        }

        NumN index_tensor_to_array(NumN j1) const {
            return j1;
        }

        NumN index_tensor_to_array(NumN j1, NumN j2) const {
            if (dims() == 1) {
                MATH21_ASSERT(j2 == 1, "1-D tensor cols index not 1");
                return j1;
            }
            MATH21_ASSERT(dims() == 2, "not 2-D tensor");
            MATH21_ASSERT(j1 >= 1 && j1 <= dim(1) && j2 >= 1 && j2 <= dim(2),
                          "\tYou must give a valid index");
            return a(1)(j1) + a(2)(j2);
        }

        NumN index_tensor_to_array(NumN j1, NumN j2, NumN j3) const {
            MATH21_ASSERT(getClassName() == Tensor::getClassName(), "You must overwrite to use");
            MATH21_ASSERT(dims() == 3, "not 3-D tensor");
            MATH21_ASSERT(j1 >= 1 && j1 <= dim(1) && j2 >= 1 && j2 <= dim(2) && j3 >= 1 && j3 <= dim(3),
                          "\tYou must give a valid index");
            return a(1)(j1) + a(2)(j2) + a(3)(j3);
        }

        NumN index_tensor_to_array(NumN j1, NumN j2, NumN j3, NumN j4) const {
            MATH21_ASSERT(dims() == 4, "not 4-D tensor");
            MATH21_ASSERT(
                    j1 >= 1 && j1 <= dim(1) && j2 >= 1 && j2 <= dim(2) && j3 >= 1 && j3 <= dim(3) && j4 >= 1 &&
                    j4 <= dim(4),
                    "\tYou must give a valid index");
            return a(1)(j1) + a(2)(j2) + a(3)(j3) + a(4)(j4);
        }

        NumN index_tensor_to_array(NumN j1, NumN j2, NumN j3, NumN j4, NumN j5) const {
            MATH21_ASSERT(dims() == 5, "not 5-D tensor");
            MATH21_ASSERT(
                    j1 >= 1 && j1 <= dim(1) && j2 >= 1 && j2 <= dim(2) && j3 >= 1 && j3 <= dim(3) && j4 >= 1 &&
                    j4 <= dim(4) && j5 >= 1 && j5 <= dim(5),
                    "\tYou must give a valid index");
            return a(1)(j1) + a(2)(j2) + a(3)(j3) + a(4)(j4) + a(5)(j5);
        }

        NumN index_tensor_to_array(NumN j1, NumN j2, NumN j3, NumN j4, NumN j5, NumN j6) const {
            MATH21_ASSERT(getClassName() == Tensor::getClassName(), "You must overwrite to use");
            MATH21_ASSERT(dims() == 6, "not 6-D tensor");
            MATH21_ASSERT(
                    j1 >= 1 && j1 <= dim(1) && j2 >= 1 && j2 <= dim(2) && j3 >= 1 && j3 <= dim(3) && j4 >= 1 &&
                    j4 <= dim(4) && j5 >= 1 && j5 <= dim(5) && j6 >= 1 && j6 <= dim(6),
                    "\tYou must give a valid index");
            return a(1)(j1) + a(2)(j2) + a(3)(j3) + a(4)(j4) + a(5)(j5) + a(6)(j6);
        }

    public:

        void clear() {
            clearSome();
            is_column_major = 0;
        }

        // not test
        void setColumnMajor(NumB isColumnMajor) {
            is_column_major = isColumnMajor;
            if (isEmpty()) {
                return;
            }
            NumN scale = 0;
            _setSize_index(scale);
        }

        NumB isColumnMajor() const {
            return is_column_major;
        }

        void init() {
            clear();
        }

        //Construct empty matrix.
        Tensor() {
            init();
        }


        //copy constructor.
        // Now we allow copy constructor.
        Tensor(const Tensor &B) {
//            MATH21_ASSERT(0)
            init();
            setColumnMajor(B.isColumnMajor());
            copyFrom(B);
        }

        //construct like mlp, but from top to bottom.
        Tensor(ArrayN d) {
            init();
            setSize(d);
        }

        //from left to right, d1 is lowest.
        Tensor(NumN d1) {
            init();
            setSize(d1);
        }

        //from left to right, d2 is lowest.
        Tensor(NumN d1, NumN d2) {
            init();
            setSize(d1, d2);
        }

        //from left to right, d3 is lowest.
        Tensor(NumN d1, NumN d2, NumN d3) {
            init();
            setSize(d1, d2, d3);
        }

        //from left to right, d4 is lowest.
        Tensor(NumN d1, NumN d2, NumN d3, NumN d4) {
            init();
            setSize(d1, d2, d3, d4);
        }

        //from left to right, d1 is lowest.
        void setSize(NumN d1) {
            if (isSameSize(d1)) {
                return;
            }
            ArrayN d(1);
            d = d1;
            _setSize(d);
        }

        //from left to right, d2 is lowest.
        void setSize(NumN d1, NumN d2) {
            if (isSameSize(d1, d2)) {
                return;
            }
            ArrayN d(2);
            d = d1, d2;
            _setSize(d);
        }

        //from left to right, d3 is lowest.
        void setSize(NumN d1, NumN d2, NumN d3) {
            if (isSameSize(d1, d2, d3)) {
                return;
            }
            ArrayN d(3);
            d = d1, d2, d3;
            _setSize(d);
        }

        //from left to right, d4 is lowest.
        void setSize(NumN d1, NumN d2, NumN d3, NumN d4) {
            if (isSameSize(d1, d2, d3, d4)) {
                return;
            }
            ArrayN d(4);
            d = d1, d2, d3, d4;
            _setSize(d);
        }

        void setSize(const ArrayN &_d) {
            if (isSameSize(_d)) {
                return;
            }
            _setSize(_d);
        }

        // reshape not sharing. See share reshape.
        //Tensor<NumN> is VecN actually.
        Tensor &reshape(const Tensor<NumN> &d) {
            MATH21_ASSERT_CODE(d.dims() == 1, "d must be type VecN.")
            ArrayN _d;
            _d.setSize(d.size());
            math21_operator_vec_2_array(d, _d);
            _setSizeDataFixed(_d);
            return *this;
        }

        Tensor &toVector() {
            Tensor<NumN> d(1);
            d = volume();
            return reshape(d);
        }

        // deprecate, why?
        //Tensor<NumN> is VecN actually.
        void setSize(const Tensor<NumN> &d, const SpaceParas *paras = 0) {
            MATH21_ASSERT_CODE(d.dims() == 1, "d must be type VecN.")
            ArrayN _d;
            _d.setSize(d.size());
            math21_operator_vec_2_array(d, _d);
            if (isSameSize(_d) && paras == 0) {
                return;
            }
            _setSize(_d, paras);
        }

        // keep size fixed, just set space.
        void setSpace(const SpaceParas &paras) {
            Tensor<NumN> d;
            d.setSize(dims());
            math21_operator_array_2_vec(shape(), d);
            setSize(d, &paras);
        }

        void setSizeNoSpace(const Tensor<NumN> &d) {
            MATH21_ASSERT_CODE(d.dims() == 1, "d must be type VecN.")
            clearSome();
            ArrayN _d;
            _d.setSize(d.size());
            math21_operator_vec_2_array(d, _d);
            if (isSameSize(_d)) {
                return;
            }
            _setSizeNoSpace(_d);
        }

        NumB isEmpty() const {
            return N == 0 ? (NumB) 1 : (NumB) 0;
        }

        virtual ~Tensor() {
            clear();
        }

        // deprecate
        virtual T &operator()(const ArrayN &index) {
            MATH21_ASSERT(getClassName() == Tensor::getClassName(), "You must overwrite to use");
            return data.at(index_tensor_to_array(index));
        }

        // deprecate, use virtual const T &operator()(const Tensor<NumN> &index);
        virtual const T &operator()(const ArrayN &index) const {
            MATH21_ASSERT(getClassName() == Tensor::getClassName(), "You must overwrite to use"
                    << "\n\tgetClassName(): " << getClassName());
            return data(index_tensor_to_array(index));
        }

        //index from high to low: index of 4322 is 4322, not 2234
        virtual T &operator()(const Tensor<NumN> &index) {
            MATH21_ASSERT(getClassName() == Tensor::getClassName(), "You must overwrite to use");
            return data.at(index_tensor_to_array(index));
        }

        virtual const T &operator()(const Tensor<NumN> &index) const {
            MATH21_ASSERT(getClassName() == Tensor::getClassName(), "You must overwrite to use"
                    << "\n\tgetClassName(): " << getClassName());
            return data(index_tensor_to_array(index));
        }

        template<template<typename> class Container, typename NumNType>
        T &operator()(const Container<NumNType> &index_a, NumNType j1) {
            Tensor<NumN> index(index_a.size() + 1);
            math21_operator_container_set_partially(index_a, index, 0, 0, index_a.size());
            index(index.size()) = j1;
            return this->operator()(index);
        }

        template<template<typename> class Container, typename NumNType>
        const T &operator()(const Container<NumNType> &index_a, NumNType j1) const {
            Tensor<NumN> index(index_a.size() + 1);
            math21_operator_container_set_partially(index_a, index, 0, 0, index_a.size());
            index.at(index.size()) = j1;
            return this->operator()(index);
        }

        // see tensor as container.
        virtual T &at(NumN j1) {
            MATH21_ASSERT(getClassName() == Tensor::getClassName(), "You must overwrite to use");
            return data.at(index_tensor_to_array(j1));
        }

        virtual T &operator()(NumN j1) {
            MATH21_ASSERT(getClassName() == Tensor::getClassName(), "You must overwrite to use");
            return data.at(index_tensor_to_array(j1));
        }

        virtual const T &operator()(NumN j1) const {
            MATH21_ASSERT(getClassName() == Tensor::getClassName(), "You must overwrite to use");
            return data(index_tensor_to_array(j1));
        }

        virtual T &operator()(NumN j1, NumN j2) {
            MATH21_ASSERT(getClassName() == Tensor::getClassName(), "You must overwrite to use");
            return data.at(index_tensor_to_array(j1, j2));
        }

        virtual const T &operator()(NumN j1, NumN j2) const {
            MATH21_ASSERT(getClassName() == Tensor::getClassName(), "You must overwrite to use");
            return data(index_tensor_to_array(j1, j2));
        }

        T &valueAt(NumN j1, NumN j2, NumN j3) {
            return this->operator()(j1, j2, j3);
        }

        virtual T &operator()(NumN j1, NumN j2, NumN j3) {
            MATH21_ASSERT(getClassName() == Tensor::getClassName(), "You must overwrite to use");
            return data.at(index_tensor_to_array(j1, j2, j3));
        }

        virtual const T &operator()(NumN j1, NumN j2, NumN j3) const {
            MATH21_ASSERT(getClassName() == Tensor::getClassName(), "You must overwrite to use");
            return data(index_tensor_to_array(j1, j2, j3));
        }

        virtual T &operator()(NumN j1, NumN j2, NumN j3, NumN j4) {
            MATH21_ASSERT(getClassName() == Tensor::getClassName(), "You must overwrite to use");
            return data.at(index_tensor_to_array(j1, j2, j3, j4));
        }

        virtual const T &operator()(NumN j1, NumN j2, NumN j3, NumN j4) const {
            MATH21_ASSERT(getClassName() == Tensor::getClassName(), "You must overwrite to use");
            return data(index_tensor_to_array(j1, j2, j3, j4));
        }

        virtual T &operator()(NumN j1, NumN j2, NumN j3, NumN j4, NumN j5) {
            MATH21_ASSERT(getClassName() == Tensor::getClassName(), "You must overwrite to use");
            return data.at(index_tensor_to_array(j1, j2, j3, j4, j5));
        }

        virtual const T &operator()(NumN j1, NumN j2, NumN j3, NumN j4, NumN j5) const {
            MATH21_ASSERT(getClassName() == Tensor::getClassName(), "You must overwrite to use");
            return data(index_tensor_to_array(j1, j2, j3, j4, j5));
        }

        virtual T &operator()(NumN j1, NumN j2, NumN j3, NumN j4, NumN j5, NumN j6) {
            MATH21_ASSERT(getClassName() == Tensor::getClassName(), "You must overwrite to use");
            return data.at(index_tensor_to_array(j1, j2, j3, j4, j5, j6));
        }

        virtual const T &operator()(NumN j1, NumN j2, NumN j3, NumN j4, NumN j5, NumN j6) const {
            MATH21_ASSERT(getClassName() == Tensor::getClassName(), "You must overwrite to use");
            return data(index_tensor_to_array(j1, j2, j3, j4, j5, j6));
        }

        NumN dims() const {
            return N;
        }

        // for vector, matrix and tensor.
        NumN size() const {
            if (isEmpty()) {
                return 0;
            }
            return volume();
        }

        NumB isScalar() const {
            if (dims() == 1 && volume() == 1) {
                return 1;
            }
            return 0;
        }

        NumN nrows() const {
            if (isEmpty()) {
                return 0;
            }
            MATH21_ASSERT(0 < dims() && dims() <= 2, "not 2-D tensor");
            return dim(1);
        }

        NumN ncols() const {
            if (isEmpty()) {
                return 0;
            }
            MATH21_ASSERT(0 < dims() && dims() <= 2, "not 2-D tensor");
            if (dims() == 1) {
                return 1;
            }
            return dim(2);
        }

        NumN dim(NumN i) const {
            return d(i);
        }

        //from left to right.
        const ArrayN &shape() const {
            return d;
        }

        //from left to right.
        const Tensor<NumN> &shape(Tensor<NumN> &d2) const {
            if (!d2.isSameSize(d.size())) {
                d2.setSize(d.size());
            }
            math21_operator_array_2_vec(shape(), d2);
            return d2;
        }

        // return how many numbers.
        NumN volume() const {
            if (isEmpty()) {
                return 0;
            }
            return math21_operator_container_multiply_all(d);
        }

        virtual std::string getClassName() const {
            return "Tensor";
        }


        void setName(const char *_name) {
            name = _name;
        }

        const std::string &getName() const {
            return name;
        }

        NumB log(const char *name = 0, NumB isUsingCommas = 0, NumB isLogDetail = 1, NumN precision = 3) const;

        NumB log(std::ostream &io, const char *name = 0, NumB isUsingCommas = 0, NumB isLogDetail = 1,
                 NumN precision = 3) const;

        NumB logInfo(const char *name = "") const;

        NumB logInfo(std::ostream &io, const char *name = "") const;

        virtual NumB isWritable() const {
            return (NumB) 1;
        }

        NumB isSameSize(const NumN &n) const {
            if (dims() == 1 && dim(1) == n) {
                return 1;
            } else {
                return 0;
            }
        }

        NumB isSameSize(const NumN &n, const NumN &m) const {
            if (dims() == 2 && dim(1) == n && dim(2) == m) {
                return 1;
            } else {
                return 0;
            }
        }

        NumB isSameSize(NumN d1, NumN d2, NumN d3) const {
            if (dims() == 3 && dim(1) == d1 && dim(2) == d2 && dim(3) == d3) {
                return 1;
            } else {
                return 0;
            }
        }

        NumB isSameSize(NumN d1, NumN d2, NumN d3, NumN d4) const {
            if (dims() == 4 && dim(1) == d1 && dim(2) == d2 && dim(3) == d3 && dim(4) == d4) {
                return 1;
            } else {
                return 0;
            }
        }

        NumB isSameSize(const ArrayN &d) const {
            return math21_operator_container_isEqual(shape(), d);
        }

        NumB isSameSize(const Tensor<NumN> &d) const {
            MATH21_ASSERT_CODE(d.dims() == 1, "d must be type VecN.")
            ArrayN _d;
            _d.setSize(d.size());
            math21_operator_vec_2_array(d, _d);
            return isSameSize(_d);
        }

        NumB isSameSizeVirtually(const ArrayN &d) const {
            if (dims() == 1 && d.size() == 2) {
                if (d(1) == dim(1) && d(2) == 1) {
                    return 1;
                } else {
                    return 0;
                }
            } else if (dims() == 2 && d.size() == 1) {
                if (d(1) == dim(1) && dim(2) == 1) {
                    return 1;
                } else {
                    return 0;
                }
            }
            return isSameSize(d);
        }

        void assign(const Tensor<T> &B);

        template<typename S>
        void assign(const S &a) {
            MATH21_ASSERT(isBasicType(a))
            data.assign((const T &) a);
        }

        // yes, this is just assignment.
        Tensor &operator=(const Tensor &B) {
//            MATH21_ASSERT(0)
            assign(B);
            return *this;
        }

        void copyTo(Tensor &B) const {
            B.setSize(shape());
            B.assign(*this);
        }

        void copyFrom(const Tensor &B) {
            setSize(B.shape());
            assign(B);
        }

        virtual NumB isContinuous() const {
            return 1;
        }

        // deprecated
        SpaceParas getSpace(NumN offset, NumN size, NumN unit = sizeof(char)) const {
            return data.getAutoBuffer().getSpace(offset, size, unit);
        }

        // deprecated
        SpaceParas getSpace() const {
            return data.getAutoBuffer().getSpace();
        }

//        const detail::literal_assign_helper_tensor<T> operator=(
//                const T &val
//        ) {
//            // assign the given value to every spot in this matrix
//            assign(val);
//            // Now return the literal_assign_helper so that the user
//            // can use the overloaded comma notation to initialize
//            // the matrix if they want to.
//            return detail::literal_assign_helper_tensor<T>(*this);
//        }
        template<typename S>
        const detail::literal_assign_helper_tensor<T> operator=(
                const S &val
        ) {
            // assign the given value to every spot in this matrix
//            assign((const T&) val);
            assign(val);
            // Now return the literal_assign_helper so that the user
            // can use the overloaded comma notation to initialize
            // the matrix if they want to.
            return detail::literal_assign_helper_tensor<T>(*this);
        }

        void letters() {
            math21_operator_container_letters(data);
        }

        //Tensor<NumN> is VecN
        TensorView<T> sliceView(const Seqce<Tensor<NumN> > &index) const;

        TensorSub<T> sliceSub(const Seqce<Tensor<NumN> > &index);

        //Tensor<NumN> is VecN
        TensorView<T> shrinkView(const Tensor<NumN> &index) const;

        //Tensor<NumN> is VecN
        TensorSub<T> shrinkSub(const Tensor<NumN> &index);

        void swap(Tensor &B) {
            MATH21_ASSERT(getClassName() == Tensor::getClassName(), "You must overwrite to use");
            data.swap(B.data);
            a.swap(B.a);
            d.swap(B.d);
            m21_swap(N, B.N);
            m21_swap(is_column_major, B.is_column_major);
        }

        void zeros() {
            if (!isEmpty()) {
                assign(0);
            }
        }

        void getIndex(NumN i, Tensor<NumN> &index) const {
            math21_operator_number_index_1d_to_tensor_nd(index, i, shape());
        }
    };

    template<typename T>
    NumB Tensor<T>::logInfo(const char *name) const {
        return logInfo(std::cout, name);
    }

    template<typename T>
    NumB Tensor<T>::logInfo(std::ostream &io, const char *name) const {
        if (!name) {
            name = "";
        }
        io << getClassName() << " " << (getName() + std::string(name)) << ", dims: " << dims() << "\n";
        io << "architecture (from left to right): ";
        shape().log(io, 0, 0);
        io << "isColumnMajor: " << isColumnMajor() << "\n";
//        shape().log("shape", 0);
        if (isLogAutoBuffer()) {
            data.getAutoBuffer().log(io, name);
        }
        return 1;
    }

    template<typename T>
    std::ostream &operator<<(std::ostream &out, const Tensor<T> &m) {
        m.log(out);
        return out;
    }

    typedef Tensor<NumN> TenN;
    typedef Tensor<NumZ> TenZ;
    typedef Tensor<NumR> TenR;
}