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

#include "operations.h"
#include "ten_ops.h"

namespace math21 {

    /* RULES: EVERY METHODS MUST OBEY!!!
     * RULE 1: if method input is A and output is A, then we must guarantee A has the same type.
     * i.e., if A is vector when as input, it will still be vector when as output.
     * */


    void math21_operator_vec_linear(NumR k1, const VecR &A, VecR &C) {
        math21_operator_linear(k1, A, C);
        MATH21_ASSERT_CODE(C.dims() == 1, "C is not vector");
    }


    // We require that C must be vector if both A and B are vectors.
    void math21_operator_vec_linear(NumR k1, const VecR &A, NumR k2, const VecR &B, VecR &C) {
        math21_operator_linear(k1, A, k2, B, C);
        MATH21_ASSERT_CODE(C.dims() == 1, "C is not vector");
    }

    // A = k1*A
    void math21_operator_linear_to(NumR k1, TenR &A) {
        math21_operator_linear(k1, A, A);
    }

    // Note: if A is be vector, then output A will be vector. This is guaranteed.
    // A = k1*A + k2*B
    void math21_operator_linear_to_A(NumR k1, MatR &A, NumR k2, const MatR &B) {
        math21_operator_linear(k1, A, k2, B, A);
    }

    // Note: if B is be vector, then output B will be vector. This is guaranteed.
    // B = k1*A + k2*B
    void math21_operator_linear_to_B(NumR k1, const MatR &A, NumR k2, MatR &B) {
        math21_operator_linear(k1, A, k2, B, B);
    }

    // solve A*X=B;
    void math21_operator_solve_linear_equation(const MatR &A, const MatR &B, MatR &X) {
        MatR A_inv;
        A_inv.setSize(A.shape());
        A_inv.assign(A);
        if (!math21_operator_container_isEqual(X.shape(), B.shape())) {
            X.setSize(B.shape());
        }
        X.assign(B);
        GaussJordanElimination gje;
        gje.solve(A_inv, X);
    }

    void math21_operator_inverse(const MatR &A, MatR &A_inv) {
        A_inv.setSize(A.shape());
        A_inv.assign(A);

        GaussJordanElimination gje;
        gje.solve(A_inv);
    }

    void math21_operator_inverse(MatR &A) {
        GaussJordanElimination gje;
        gje.solve(A);
    }

    // B = inverse of A
    void math21_operator_matrix_2_2_inverse(const MatR &A, MatR &B) {
        MATH21_ASSERT(A.isSameSize(2, 2))
        if (!B.isSameSize(2, 2)) {
            B.setSize(2, 2);
        }
        NumR a, b, c, d, det;
        det = A(1, 1) * A(2, 2) - A(2, 1) * A(1, 2);
        MATH21_ASSERT(det != 0)
        a = A(2, 2) / det;
        b = -A(2, 1) / det;
        c = -A(1, 2) / det;
        d = A(1, 1) / det;
        B =
                a, c,
                b, d;
    }

    // B = inverse of A
    void math21_operator_matrix_2_2_inverse(MatR &A) {
        math21_operator_matrix_2_2_inverse(A, A);
    }


    int math21_compute_num_threads(int n, int min_n) {
#ifdef MATH21_FLAG_IS_PARALLEL
#ifdef MATH21_FLAG_USE_OPENMP
        int max_tn = n / min_n;
        const int g_ncore = omp_get_num_procs();
        int tn = max_tn > g_ncore ? g_ncore : max_tn;
        if (tn < 1) {
            tn = 1;
        }
        printf("tn:%d\n", tn);
        return tn;
#else
        return 1;
#endif
#else
        return 1;
#endif
    }

    // A = s*A*B, * is matrix multiplication
    void math21_operator_multiply_to_A(NumR s, MatR &A, const MatR &B) {
        MatR C;
        math21_operator_multiply(s, A, B, C);
        A.swap(C);
    }

    // B = s*A*B, * is matrix multiplication
    void math21_operator_multiply_to_B(NumR s, const MatR &A, MatR &B) {
        MatR C;
        math21_operator_multiply(s, A, B, C);
        B.swap(C);
    }

    // C = s*(A.transpose)*B, * is matrix multiplication
    void math21_operator_trans_multiply(NumR s, const MatR &A, const MatR &B, MatR &C) {
        MATH21_ASSERT(!A.isEmpty() && !B.isEmpty(), "empty matrix");
        MATH21_ASSERT(B.nrows() == A.nrows(), "matrix size doesn't match in *");

        NumN n, m, r;
        n = A.ncols();
        m = B.ncols();
        r = A.nrows();
        if (C.nrows() != n || C.ncols() != m) {
            C.setSize(n, m);
        }

        NumN i, j, k;
        for (i = 1; i <= n; i++) {
            for (j = 1; j <= m; j++) {
                C(i, j) = 0;
                for (k = 1; k <= r; k++) C(i, j) += A(k, i) * B(k, j);
                C(i, j) = s * C(i, j);
            }
        }
    }

    // C = s*A*(B.transpose), * is matrix multiplication
    void math21_operator_multiply_trans(NumR s, const MatR &A, const MatR &B, MatR &C) {
        MATH21_ASSERT(!A.isEmpty() && !B.isEmpty(), "empty matrix");
        MATH21_ASSERT(B.ncols() == A.ncols(), "matrix size doesn't match in *");

        NumN n, m, r;
        n = A.nrows();
        m = B.nrows();
        r = A.ncols();
        if (C.nrows() != n || C.ncols() != m) {
            C.setSize(n, m);
        }

        NumN i, j, k;
        for (i = 1; i <= n; i++) {
            for (j = 1; j <= m; j++) {
                C(i, j) = 0;
                for (k = 1; k <= r; k++) C(i, j) += A(i, k) * B(j, k);
                C(i, j) = s * C(i, j);
            }
        }
    }

    // C = s*(A.transpose)*(B.transpose), * is matrix multiplication
    void math21_operator_trans_multiply_trans(NumR s, const MatR &A, const MatR &B, MatR &C) {
        MATH21_ASSERT(!A.isEmpty() && !B.isEmpty(), "empty matrix");
        MATH21_ASSERT(B.ncols() == A.nrows(), "matrix size doesn't match in *");

        NumN n, m, r;
        n = A.ncols();
        m = B.nrows();
        r = A.nrows();
        if (C.nrows() != n || C.ncols() != m) {
            C.setSize(n, m);
        }

        NumN i, j, k;
        for (i = 1; i <= n; i++) {
            for (j = 1; j <= m; j++) {
                C(i, j) = 0;
                for (k = 1; k <= r; k++) C(i, j) += A(k, i) * B(j, k);
                C(i, j) = s * C(i, j);
            }
        }
    }

}