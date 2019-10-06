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

    void math21_operator_vec_linear(NumR k1, const VecR &A, VecR &C);


    void math21_operator_vec_linear(NumR k1, const VecR &A, NumR k2, const VecR &B, VecR &C);

    // A = k1*A
    void math21_operator_linear_to(NumR k1, TenR &A);

    // A = k1*A + k2*B
    void math21_operator_linear_to_A(NumR k1, MatR &A, NumR k2, const MatR &B);

    // B = k1*A + k2*B
    void math21_operator_linear_to_B(NumR k1, const MatR &A, NumR k2, MatR &B);

    // solve A*X=B;
    void math21_operator_solve_linear_equation(const MatR &A, const MatR &B, MatR &X);

    void math21_operator_inverse(const MatR &A, MatR &A_inv);

    void math21_operator_inverse(MatR &A);

    // B = inverse of A
    void math21_operator_matrix_2_2_inverse(const MatR &A, MatR &B);

    // B = inverse of A
    void math21_operator_matrix_2_2_inverse(MatR &A);

    int math21_compute_num_threads(int n, int min_n);

    template<typename T>
    void math21_c_matrix_multiply_no_parallel(NumR s, const Tensor<T> &A, const Tensor<T> &B, Tensor<T> &C) {
        MATH21_ASSERT(!A.isEmpty() && !B.isEmpty(), "empty matrix");
        MATH21_ASSERT(B.nrows() == A.ncols(), "matrix size doesn't match in *");
        MATH21_ASSERT(A.isContinuous() && !A.isColumnMajor());
        MATH21_ASSERT(B.isContinuous() && !B.isColumnMajor());

        NumN n, m, r;
        n = A.nrows();
        m = B.ncols();
        r = A.ncols();
        if (C.nrows() != n || C.ncols() != m) {
            if (m == 1) {
                C.setSize(n);
            } else {
                C.setSize(n, m);
            }
        }
        MATH21_ASSERT(C.isContinuous() && !C.isColumnMajor());

        NumN i, j, k;
        const T *A_data = math21_memory_tensor_data_address(A);
        const T *B_data = math21_memory_tensor_data_address(B);
        T *C_data = math21_memory_tensor_data_address(C);
        for (i = 1; i <= n; i++) {
            for (j = 1; j <= m; j++) {
                NumR sum = 0;
                for (k = 1; k <= r; k++) sum += A_data[(i - 1) * r + (k - 1)] * B_data[(k - 1) * m + (j - 1)];
                C_data[(i - 1) * m + (j - 1)] = (T) s * sum;
            }
        }
    }

    // MATH21_ASSERT(A.isContinuous() && !A.isColumnMajor());
    // MATH21_ASSERT(B.isContinuous() && !B.isColumnMajor());
    template<typename T>
    void math21_c_matrix_multiply(NumR s, const Tensor<T> &A, const Tensor<T> &B, Tensor<T> &C) {
        MATH21_ASSERT(!A.isEmpty() && !B.isEmpty(), "empty matrix");
        MATH21_ASSERT(B.nrows() == A.ncols(), "matrix size doesn't match in *");

        NumN n, m, r;
        n = A.nrows();
        m = B.ncols();
        r = A.ncols();
        if (C.nrows() != n || C.ncols() != m) {
            if (m == 1) {
                C.setSize(n);
            } else {
                C.setSize(n, m);
            }
        }
        MATH21_ASSERT(C.isContinuous() && !C.isColumnMajor());

        NumN i, j, k;
        const T *A_data = math21_memory_tensor_data_address(A);
        const T *B_data = math21_memory_tensor_data_address(B);
        T *C_data = math21_memory_tensor_data_address(C);
#pragma omp parallel for private(j, k) collapse(2)
        for (i = 1; i <= n; i++) {
            for (j = 1; j <= m; j++) {
                NumR sum = 0;
                for (k = 1; k <= r; k++) sum += A_data[(i - 1) * r + (k - 1)] * B_data[(k - 1) * m + (j - 1)];
                C_data[(i - 1) * m + (j - 1)] = (T) s * sum;
            }
        }
    }

    // C will be vector type if C is n*1 shape.
    // C = s*A*B, * is matrix multiplication
    // data doesn't need to be continous.
    template<typename T>
    void math21_operator_matrix_multiply_general(NumR s, const Tensor<T> &A, const Tensor<T> &B, Tensor<T> &C) {
        MATH21_ASSERT(!A.isEmpty() && !B.isEmpty(), "empty matrix");
        MATH21_ASSERT(B.nrows() == A.ncols(), "matrix size doesn't match in *");

        NumN n, m, r;
        n = A.nrows();
        m = B.ncols();
        r = A.ncols();
        if (C.nrows() != n || C.ncols() != m) {
            if (m == 1) {
                C.setSize(n);
            } else {
                C.setSize(n, m);
            }
        }
        NumN i, j, k;
#pragma omp parallel for private(j, k) collapse(2) num_threads(math21_compute_num_threads(n*m, 1))
        for (i = 1; i <= n; i++) {
            for (j = 1; j <= m; j++) {
                NumR sum = 0;
                for (k = 1; k <= r; k++) sum += A(i, k) * B(k, j);
                C(i, j) = (T) s * sum;
            }
        }
    }

    // C = s*A*B, * is matrix multiplication
    template<typename T>
    void math21_operator_matrix_multiply_general_no_parallel(NumR s, const Tensor<T> &A, const Tensor<T> &B,
                                                             Tensor<T> &C) {
        MATH21_ASSERT(!A.isEmpty() && !B.isEmpty(), "empty matrix");
        MATH21_ASSERT(B.nrows() == A.ncols(), "matrix size doesn't match in *");

        NumN n, m, r;
        n = A.nrows();
        m = B.ncols();
        r = A.ncols();
        if (C.nrows() != n || C.ncols() != m) {
            if (m == 1) {
                C.setSize(n);
            } else {
                C.setSize(n, m);
            }
        }
        NumN i, j, k;
        for (i = 1; i <= n; i++) {
            for (j = 1; j <= m; j++) {
                NumR sum = 0;
                for (k = 1; k <= r; k++) sum += A(i, k) * B(k, j);
                C(i, j) = (T) s * sum;
            }
        }
    }


#ifdef MATH21_FLAG_USE_CUDA
    namespace detail {
        void _math21_c_matrix_multiply_cuda_Num(NumR s, const TenN &A, const TenN &B, TenN &C);

        void _math21_c_matrix_multiply_cuda_Num(NumR s, const TenZ &A, const TenZ &B, TenZ &C);

        void _math21_c_matrix_multiply_cuda_Num(NumR s, const TenR &A, const TenR &B, TenR &C);

        void _math21_c_matrix_multiply_cuda_Num(NumR s, const Tensor<int> &A, const Tensor<int> &B, Tensor<int> &C);
    }

    template<typename T>
    void math21_c_matrix_multiply_cuda(NumR s, const Tensor <T> &A, const Tensor <T> &B, Tensor <T> &C) {
        detail::_math21_c_matrix_multiply_cuda_Num(s, A, B, C);
    }

#endif

    void math21_cuda_test_02();

    template<typename T>
    void math21_operator_multiply(NumR s, const Tensor<T> &A, const Tensor<T> &B, Tensor<T> &C) {
#ifndef MATH21_FLAG_USE_CUDA
        if (A.isContinuous() && !A.isColumnMajor()
            && B.isContinuous() && !B.isColumnMajor()) {
            math21_c_matrix_multiply(s, A, B, C);
        } else {
            math21_operator_matrix_multiply_general(s, A, B, C);
        }
#else
        math21_c_matrix_multiply_cuda(s, A, B, C);
#endif
    }

    template<typename T>
    void math21_operator_multiply_no_parallel(NumR s, const Tensor<T> &A, const Tensor<T> &B, Tensor<T> &C) {
        if (A.isContinuous() && !A.isColumnMajor()
            && B.isContinuous() && !B.isColumnMajor()) {
            math21_c_matrix_multiply_no_parallel(s, A, B, C);
        } else {
            math21_operator_matrix_multiply_general_no_parallel(s, A, B, C);
        }
    }

    // A = s*A*B, * is matrix multiplication
    void math21_operator_multiply_to_A(NumR s, MatR &A, const MatR &B);

    // B = s*A*B, * is matrix multiplication
    void math21_operator_multiply_to_B(NumR s, const MatR &A, MatR &B);

    void math21_operator_trans_multiply(NumR s, const MatR &A, const MatR &B, MatR &C);

    void math21_operator_multiply_trans(NumR s, const MatR &A, const MatR &B, MatR &C);

    void math21_operator_trans_multiply_trans(NumR s, const MatR &A, const MatR &B, MatR &C);

}