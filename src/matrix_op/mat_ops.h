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

    void math21_operator_mat_eye(MatR &A);

    // A is type Seqce<Vec>, B is Mat
    template<typename T>
    void math21_operator_seqce_to_mat(const Seqce <Tensor<T>> &A, Tensor <T> &B) {
        if (A.isEmpty()) {
            return;
        }
        NumN nr = A.size();
        NumN nc = A(1).size();
        if (B.isSameSize(nr, nc) == 0) {
            B.setSize(nr, nc);
        }
        for (NumN k = 1; k <= nr; ++k) {
            math21_operator_matrix_row_set(B, k, A(k));
        }
    }

    template<typename T>
    void math21_operator_swap_rows(Tensor <T> &A, NumN i, NumN j) {
        MATH21_ASSERT(i <= A.nrows() && j <= A.nrows());
        if (i == j) {
            return;
        }
        NumN n = A.ncols();
        NumN k;
        for (k = 1; k <= n; ++k) {
            m21_swap(A.operator()(i, k), A.operator()(j, k));
        }
    }

    template<typename T>
    void math21_operator_swap_cols(Tensor <T> &A, NumN i, NumN j) {
        MATH21_ASSERT(i <= A.ncols() && j <= A.ncols());
        if (i == j) {
            return;
        }
        NumN n = A.nrows();
        NumN k;
        for (k = 1; k <= n; ++k) {
            m21_swap(A.operator()(k, i), A.operator()(k, j));
        }
    }

    template<typename T>
    void math21_operator_matrix_row_set_value(Tensor <T> &A, NumN i, T x) {
        MATH21_ASSERT(i <= A.nrows());
        NumN n = A.ncols();
        NumN k;
        for (k = 1; k <= n; ++k) {
            A(i, k) = x;
        }
    }

    // A is Mat, x is Vec
    template<typename T>
    void math21_operator_matrix_row_set(Tensor <T> &A, NumN i, const Tensor <T> &x) {
        MATH21_ASSERT(i <= A.nrows() && x.size() == A.ncols());
        NumN n = A.ncols();
        NumN k;
        for (k = 1; k <= n; ++k) {
            A(i, k) = x(k);
        }
    }

    // set row i in mat A by row j in mat B
    template<typename T>
    void math21_operator_matrix_row_set(Tensor <T> &A, NumN i, const Tensor <T> &B, NumN j) {
        MATH21_ASSERT(i <= A.nrows() && A.ncols() == B.ncols() && j <= B.nrows());
        NumN n = A.ncols();
        NumN k;
        for (k = 1; k <= n; ++k) {
            A(i, k) = B(j, k);
        }
    }

    // set col i in mat A by col j in mat B
    template<typename T>
    void math21_operator_matrix_col_set(Tensor <T> &A, NumN i, const Tensor <T> &B, NumN j) {
        MATH21_ASSERT(i <= A.ncols() && A.nrows() == B.nrows() && j <= B.ncols());
        NumN n = A.nrows();
        NumN k;
        for (k = 1; k <= n; ++k) {
            A(k, i) = B(k, j);
        }
    }

    template<typename T>
    void math21_operator_matrix_col_set_value(Tensor <T> &A, NumN i, T x) {
        MATH21_ASSERT(i <= A.ncols());
        NumN n = A.nrows();
        NumN k;
        for (k = 1; k <= n; ++k) {
            A(k, i) = x;
        }
    }

    template<typename T>
    void math21_operator_matrix_col_set(Tensor <T> &A, NumN i, const Tensor <T> &x) {
        MATH21_ASSERT(i <= A.ncols() && x.size() == A.nrows());
        NumN n = A.nrows();
        NumN k;
        for (k = 1; k <= n; ++k) {
            A(k, i) = x(k);
        }
    }

    template<typename T>
    void math21_operator_tensor_slice(const Tensor <T> &A, Tensor <T> &B, const Seqce <VecN> &X) {
        TensorView<T> tv = A.sliceView(X);
        tv.toTensor(B);
    }

    struct TensorFunction_sum {
        TensorFunction_sum() {
        }

        template<typename T>
        NumR compute(const Tensor <T> &A) {
            return math21_operator_container_sum(A, 1);
        }
    };

    struct TensorFunction_min {
        TensorFunction_min() {
        }

        template<typename T>
        T compute(const Tensor <T> &A) {
            return math21_operator_container_min(A);
        }
    };

    struct TensorFunction_max {
        TensorFunction_max() {
        }

        template<typename T>
        T compute(const Tensor <T> &A) {
            return math21_operator_container_max(A);
        }
    };

    struct TensorFunction_argmax {
        TensorFunction_argmax() {
        }

        template<typename T>
        NumN compute(const Tensor <T> &A) {
            return math21_operator_container_argmax(A);
        }
    };

    struct TensorFunction_argmax_random {
        think::RandomEngine &engine;

        TensorFunction_argmax_random(think::RandomEngine &engine) : engine(engine) {
        }

        template<typename T>
        NumN compute(const Tensor <T> &A) {
            return math21_operator_container_argmax_random(A, engine);
        }
    };

    struct TensorFunction_argmin {
        TensorFunction_argmin() {
        }

        template<typename T>
        NumN compute(const Tensor <T> &A) {
            return math21_operator_container_argmin(A);
        }
    };

    struct TensorFunction_mean {
        template<typename T>
        NumR compute(const Tensor <T> &A) {
            return math21_operator_container_mean(A);
        }
    };

    template<typename T>
    void math21_operator_tensor_shrink(const Tensor <T> &A, Tensor <T> &B, const VecN &X) {
        TensorView<T> tv = A.shrinkView(X);
        tv.toTensor(B);
    }

    // X(i) is 1 or 0.
    // 1 variable, 0 parameter.
    template<typename T, typename S, typename TensorFunction>
    void math21_operator_tensor_f_shrink(const Tensor <T> &A, Tensor <S> &B, const VecN &X, TensorFunction &f) {
        VecN b(X.size());
        b = 1;
        math21_operator_container_linear_to_A(1, b, -1, X);

        // get parameter shape
        VecN dp;
        math21_operator_tensor_shrink_shape(A, X, dp);
        if (dp.isEmpty()) {
            B.setSize(1);
//            B(1) = math21_operator_container_sum(A, 1);
            B(1) = f.compute(A);
            return;
        }
        MATH21_ASSERT(dp.size() != A.dims(), "No variable specified!")
        B.setSize(dp);

        VecN &d = dp;
        d = 1;
        VecN y(b.size());
        while (1) {
            math21_operator_container_replace(b, y, d, (NumN) 1);
            TensorView<T> A_prime = A.shrinkView(y);
//            B(d) = math21_operator_container_sum(A_prime, 1);
            B(d) = f.compute(A_prime);
            if (math21_operator_container_increaseNumFromRight(B.shape(), d) == 0) {
                break;
            }
        }
    }

    template<typename T>
    void
    math21_operator_matrix_submatrix(const Tensor <T> &A, Tensor <T> &B, const VecN &row_indexes,
                                     const VecN &col_indexes) {
        MATH21_ASSERT(A.dims() == 2)
        Seqce<VecN> X;
        X.setSize(2);
        X(1).copyFrom(row_indexes);
        X(2).copyFrom(col_indexes);
        math21_operator_tensor_slice(A, B, X);
    }


    // delete i-th row. i= 1, ..., nrow.
    template<typename T>
    void math21_operator_matrix_delete_row(const Tensor <T> &A, Tensor <T> &B, NumN i) {
        if (A.isEmpty() || A.nrows() == 1) {
            return;
        }
        MATH21_ASSERT(i <= A.nrows() && i >= 1);
        if (B.isSameSize(A.nrows() - 1, A.ncols()) == 0) {
            B.setSize(A.nrows() - 1, A.ncols());
        }
        NumN n = A.nrows();
        NumN k;
        for (k = 1; k <= n; ++k) {
            if (k < i) {
                math21_operator_matrix_row_set(B, k, A, k);
            } else if (k == i) {
            } else {
                math21_operator_matrix_row_set(B, k - 1, A, k);
            }
        }
    }

    // delete i-th row. i= 1, ..., ncol.
    template<typename T>
    void math21_operator_matrix_delete_col(const Tensor <T> &A, Tensor <T> &B, NumN i) {
        if (A.isEmpty() || A.ncols() == 1) {
            return;
        }
        MATH21_ASSERT(i <= A.ncols() && i >= 1);
        if (B.isSameSize(A.nrows(), A.ncols() - 1) == 0) {
            B.setSize(A.nrows(), A.ncols() - 1);
        }
        NumN n = A.ncols();
        NumN k;
        for (k = 1; k <= n; ++k) {
            if (k < i) {
                math21_operator_matrix_col_set(B, k, A, k);
            } else if (k == i) {
            } else {
                math21_operator_matrix_col_set(B, k - 1, A, k);
            }
        }
    }

    // insert new row to i-th row. i= 1, ..., nrow, nrow+1.
    template<typename T>
    void math21_operator_matrix_insert_row_value(const Tensor <T> &A, Tensor <T> &B, NumN i, T x) {
        if (A.isEmpty()) {
            return;
        }
        MATH21_ASSERT(i <= A.nrows() + 1 && i >= 1);
        if (A.dims() == 1) {
            if (B.isSameSize(A.nrows() + 1) == 0) {
                B.setSize(A.nrows() + 1);
            }
        } else if (B.isSameSize(A.nrows() + 1, A.ncols()) == 0) {
            B.setSize(A.nrows() + 1, A.ncols());
        }
        NumN n = B.nrows();
        NumN k;
        for (k = 1; k <= n; ++k) {
            if (k < i) {
                math21_operator_matrix_row_set(B, k, A, k);
            } else if (k == i) {
                math21_operator_matrix_row_set_value(B, k, x);
            } else {
                math21_operator_matrix_row_set(B, k, A, k - 1);
            }
        }
    }

    // insert new row to i-th row. i= 1, ..., nrow, nrow+1.
    template<typename T>
    void math21_operator_vec_insert_value(const Tensor <T> &A, Tensor <T> &B, NumN i, T x) {
        if (A.isEmpty()) {
            MATH21_ASSERT(i == 1)
            B.setSize(1);
            B = x;
            return;
        }
        math21_operator_matrix_insert_row_value(A, B, i, x);
    }

    // insert new col to i-th col. i= 1, ..., ncol, ncol+1.
    template<typename T>
    void math21_operator_matrix_insert_col_value(const Tensor <T> &A, Tensor <T> &B, NumN i, T x) {
        if (A.isEmpty()) {
            return;
        }
        MATH21_ASSERT(i <= A.ncols() + 1 && i >= 1);
        if (B.isSameSize(A.nrows(), A.ncols() + 1) == 0) {
            B.setSize(A.nrows(), A.ncols() + 1);
        }
        NumN n = B.ncols();
        NumN k;
        for (k = 1; k <= n; ++k) {
            if (k < i) {
                math21_operator_matrix_col_set(B, k, A, k);
            } else if (k == i) {
                math21_operator_matrix_col_set_value(B, k, x);
            } else {
                math21_operator_matrix_col_set(B, k, A, k - 1);
            }
        }
    }

    // A is Mat
    template<typename T>
    void math21_operator_matrix_diagonal_set(Tensor <T> &A, const Tensor <T> &x) {
        NumN n = xjmin(A.nrows(), A.ncols());
        MATH21_ASSERT(x.size() == n);
        NumN k;
        for (k = 1; k <= n; ++k) {
            A(k, k) = x(k);
        }
    }

    // A is Mat
    template<typename T>
    void math21_operator_matrix_diagonal_set_value(Tensor <T> &A, NumR x) {
        NumN n = xjmin(A.nrows(), A.ncols());
        NumN k;
        for (k = 1; k <= n; ++k) {
            A(k, k) = x;
        }
    }


    // A is Mat
    template<typename T>
    T math21_operator_matrix_sum_row_i(const Tensor <T> &A, NumN i) {
        MATH21_ASSERT(!A.isEmpty());
        NumN n = A.ncols();
        NumN k;
        T x = 0;
        for (k = 1; k <= n; ++k) {
            x += A(i, k);
        }
        return x;
    }

    // A is Mat
    template<typename T>
    T math21_operator_matrix_sum_col_i(const Tensor <T> &A, NumN i) {
        MATH21_ASSERT(!A.isEmpty());
        NumN n = A.nrows();
        NumN k;
        T x = 0;
        for (k = 1; k <= n; ++k) {
            x += A(k, i);
        }
        return x;
    }

    // A is Mat
    template<typename T>
    T math21_operator_matrix_trace(const Tensor <T> &A) {
        MATH21_ASSERT(!A.isEmpty());
        NumN n = xjmin(A.nrows(), A.ncols());
        NumN k;
        T x = 0;
        for (k = 1; k <= n; ++k) {
            x += A(k, k);
        }
        return x;
    }

    // A is Mat
    template<typename T>
    T math21_operator_matrix_reverse_trace(const Tensor <T> &A) {
        MATH21_ASSERT(!A.isEmpty());
        MATH21_ASSERT(A.nrows() == A.ncols());
        NumN n = A.nrows();
        NumN k;
        T x = 0;
        for (k = 1; k <= n; ++k) {
            x += A(k, n + 1 - k);
        }
        return x;
    }

    // A.transpose
    template<typename T>
    void math21_operator_matrix_trans(Tensor <T> &A) {
        MATH21_ASSERT(!A.isEmpty(), "empty matrix");
        MATH21_ASSERT(A.nrows() == A.ncols());
        NumN n, m;
        n = A.ncols();
        m = n;
        NumN i, j;
        for (i = 1; i <= n; i++) {
            for (j = i + 1; j <= m; j++) {
                m21_swap(A.operator()(i, j), A.operator()(j, i));
            }
        }
    }

    // B = A.transpose
    template<typename T>
    void math21_operator_matrix_trans(const Tensor <T> &A, Tensor <T> &B) {
        MATH21_ASSERT(!A.isEmpty(), "empty matrix");
        if (!B.isSameSize(A.ncols(), A.nrows())) {
            B.setSize(A.ncols(), A.nrows());
        }
        NumN nr, nc;
        NumN i, j;
        nr = B.nrows();
        nc = B.ncols();
        for (i = 1; i <= nr; ++i) {
            for (j = 1; j <= nc; ++j) {
                B(i, j) = A(j, i);
            }
        }
    }

    // A is Mat
    template<typename T>
    void math21_operator_matrix_reverse_x_axis(Tensor <T> &A) {
        MATH21_ASSERT(!A.isEmpty());

        NumN nr = A.nrows();
        NumN nc = A.ncols();
        NumN j;

        NumN n = nr;
        NumN n2 = n / 2;
        for (j = 1; j <= nc; ++j) {
            for (NumN k = 1; k <= n2; ++k) {
                m21_swap(A(k, j), A(n + 1 - k, j));
            }
        }
    }

    // A is Mat
    template<typename T>
    void math21_operator_matrix_reverse_x_axis(const Tensor <T> &A, Tensor <T> &B) {
        MATH21_ASSERT(!A.isEmpty());
        if (!B.isSameSize(A.shape())) {
            B.setSize(A.shape());
        }
        NumN nr = A.nrows();
        NumN nc = A.ncols();
        NumN i, j;
        for (i = 1; i <= nr; ++i) {
            for (j = 1; j <= nc; ++j) {
                B(i, j) = A(nr + 1 - i, j);
            }
        }
    }

    // A is Mat
    template<typename T>
    void math21_operator_matrix_reverse_y_axis(Tensor <T> &A) {
        MATH21_ASSERT(!A.isEmpty());
        NumN nr = A.nrows();
        NumN nc = A.ncols();
        NumN j;

        NumN n = nc;
        NumN n2 = n / 2;
        for (j = 1; j <= nr; ++j) {
            for (NumN k = 1; k <= n2; ++k) {
                m21_swap(A(j, k), A(j, n + 1 - k));
            }
        }
    }

    // A is Mat
    template<typename T>
    void math21_operator_matrix_reverse_y_axis(const Tensor <T> &A, Tensor <T> &B) {
        MATH21_ASSERT(!A.isEmpty());
        if (!B.isSameSize(A.shape())) {
            B.setSize(A.shape());
        }
        NumN nr = A.nrows();
        NumN nc = A.ncols();
        NumN i, j;
        for (i = 1; i <= nr; ++i) {
            for (j = 1; j <= nc; ++j) {
                B(i, j) = A(i, nc + 1 - j);
            }
        }
    }

    // A is Mat
    template<typename T>
    void math21_operator_matrix_reverse_x_axis_then_trans(const Tensor <T> &A, Tensor <T> &B) {
        MATH21_ASSERT(!A.isEmpty());
        if (!B.isSameSize(A.ncols(), A.nrows())) {
            B.setSize(A.ncols(), A.nrows());
        }
        NumN nr, nc;
        NumN i, j;
        nr = B.nrows();
        nc = B.ncols();
        for (i = 1; i <= nr; ++i) {
            for (j = 1; j <= nc; ++j) {
                B(i, j) = A(nc + 1 - j, i);
            }
        }
    }

    // A is Mat
    template<typename T>
    void math21_operator_matrix_reverse_y_axis_then_trans(const Tensor <T> &A, Tensor <T> &B) {
        MATH21_ASSERT(!A.isEmpty());
        if (!B.isSameSize(A.ncols(), A.nrows())) {
            B.setSize(A.ncols(), A.nrows());
        }
        NumN nr, nc;
        NumN i, j;
        nr = B.nrows();
        nc = B.ncols();
        for (i = 1; i <= nr; ++i) {
            for (j = 1; j <= nc; ++j) {
                B(i, j) = A(j, nr + 1 - i);
            }
        }
    }

    template<typename T>
    void math21_operator_matrix_axis_to_image(const Tensor <T> &A, Tensor <T> &B) {
        math21_operator_matrix_reverse_y_axis_then_trans(A, B);
    }
}