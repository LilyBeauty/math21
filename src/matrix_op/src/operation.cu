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

#include "inner.h"
#include "../operations.h"

namespace math21 {

#define GPU_BLOCK_SIZE 16

    // matrix multiplication
    template<typename T>
    __global__ void gpu_matrix_multiply_easy(NumR s, T *a, T *b, T *c, int n, int r, int m) {
        int row = blockIdx.y * blockDim.y + threadIdx.y;
        int col = blockIdx.x * blockDim.x + threadIdx.x;
        T sum = 0;
        if (row < n && col < m) {
            for (int i = 0; i < r; i++) {
                sum += a[row * r + i] * b[i * m + col];
            }
            c[row * m + col] = (T) s * sum;
        }
    }

    // block matrix multiplication
    template<typename T>
    __global__ void gpu_matrix_multiply_shared(NumR s, T *A, T *B, T *C,
                                               int nr_A, int nc_A, int nc_B) {
        __shared__ T sA[GPU_BLOCK_SIZE][GPU_BLOCK_SIZE];   // Tile size to store elements in shared memory
        __shared__ T sB[GPU_BLOCK_SIZE][GPU_BLOCK_SIZE];

        int row = blockDim.y * blockIdx.y + threadIdx.y; //To generate ids of threads.
        int col = blockDim.x * blockIdx.x + threadIdx.x;
        T tmp = 0;

        for (int k = 0; k < (((nc_A - 1) / GPU_BLOCK_SIZE) + 1); k++) {
            if ((row < nr_A) && (threadIdx.x + (k * GPU_BLOCK_SIZE)) < nc_A) {
                sA[threadIdx.y][threadIdx.x] = A[(row * nc_A) + threadIdx.x + (k * GPU_BLOCK_SIZE)];
            } else {
                sA[threadIdx.y][threadIdx.x] = 0;
            }
            if (col < nc_B && (threadIdx.y + k * GPU_BLOCK_SIZE) < nc_A) {
                sB[threadIdx.y][threadIdx.x] = B[(threadIdx.y + k * GPU_BLOCK_SIZE) * nc_B + col];
            } else {
                sB[threadIdx.y][threadIdx.x] = 0;
            }
            __syncthreads();

            for (int j = 0; j < GPU_BLOCK_SIZE; ++j) {
                tmp += sA[threadIdx.y][j] * sB[j][threadIdx.x];
            }
            __syncthreads();
        }
        if (row < nr_A && col < nc_B) {
            C[row * nc_B + col] = (T) s * tmp;
        }
    }

    // MATH21_ASSERT(A.isContinuous() && !A.isColumnMajor());
    // MATH21_ASSERT(B.isContinuous() && !B.isColumnMajor());
    template<typename T>
    void _math21_c_matrix_multiply_cuda(NumR s, const Tensor<T> &A, const Tensor<T> &B, Tensor<T> &C) {
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

        const T *A_data = math21_memory_tensor_data_address(A);
        const T *B_data = math21_memory_tensor_data_address(B);
        T *C_data = math21_memory_tensor_data_address(C);

        T *d_a, *d_b, *d_c;
        math21_cuda_malloc_device((void **) &d_a, sizeof(T) * n * r);
        math21_cuda_malloc_device((void **) &d_b, sizeof(T) * r * m);
        math21_cuda_malloc_device((void **) &d_c, sizeof(T) * n * m);

        // copy matrix A and B from host to device memory
        math21_cuda_memcpy_host_to_device(d_a, A_data, sizeof(T) * n * r);
        math21_cuda_memcpy_host_to_device(d_b, B_data, sizeof(T) * r * m);

        unsigned int grid_rows = (unsigned int) (n + GPU_BLOCK_SIZE - 1) / GPU_BLOCK_SIZE;
        unsigned int grid_cols = (unsigned int) (m + GPU_BLOCK_SIZE - 1) / GPU_BLOCK_SIZE;
        dim3 dimGrid(grid_cols, grid_rows, 1);
        dim3 dimBlock(GPU_BLOCK_SIZE, GPU_BLOCK_SIZE, 1);

//        timer t;
//        t.start();

        // Launch kernel
#ifdef MATH21_FLAG_UNDERSTANDABLE
        gpu_matrix_multiply_easy << < dimGrid, dimBlock >> > (s, d_a, d_b, d_c, n, r, m);
#else
        gpu_matrix_multiply_shared << < dimGrid, dimBlock >> > (s, d_a, d_b, d_c, n, r, m);
#endif

        // Transefr results from device to host
        math21_cuda_memcpy_device_to_host(C_data, d_c, sizeof(T) * n * m);
        math21_cuda_DeviceSynchronize();

        math21_cuda_free_device(d_a);
        math21_cuda_free_device(d_b);
        math21_cuda_free_device(d_c);

//        t.end();
//        printf("Time elapsed %f ms.\n\n", t.time());
    }

    namespace detail {
        void _math21_c_matrix_multiply_cuda_Num(NumR s, const TenN &A, const TenN &B, TenN &C) {
            _math21_c_matrix_multiply_cuda(s, A, B, C);
        }

        void _math21_c_matrix_multiply_cuda_Num(NumR s, const TenZ &A, const TenZ &B, TenZ &C) {
            _math21_c_matrix_multiply_cuda(s, A, B, C);
        }

        void _math21_c_matrix_multiply_cuda_Num(NumR s, const TenR &A, const TenR &B, TenR &C) {
            _math21_c_matrix_multiply_cuda(s, A, B, C);
        }

        void _math21_c_matrix_multiply_cuda_Num(NumR s, const Tensor<int> &A, const Tensor<int> &B, Tensor<int> &C) {
            _math21_c_matrix_multiply_cuda(s, A, B, C);
        }
    }
}