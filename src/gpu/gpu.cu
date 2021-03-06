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

#include <iostream>
#include "gpu.h"

namespace math21 {
    void math21_cuda_version() {
        std::cout << "CUDA Compiled version: "
                  << __CUDACC_VER_MAJOR__ << "."
                  << __CUDACC_VER_MINOR__ << "."
                  << __CUDACC_VER_BUILD__
                  << std::endl;

        int runtime_ver;
        cudaRuntimeGetVersion(&runtime_ver);
        std::cout << "CUDA Runtime version: " << runtime_ver << std::endl;

        int driver_ver;
        cudaDriverGetVersion(&driver_ver);
        std::cout << "CUDA Driver version: " << driver_ver << std::endl;
    }

    __global__ void _print_hello_world(char *a, int N) {
        char p[12] = "Hello CUDA\n";
        int idx = blockIdx.x * blockDim.x + threadIdx.x;
        if (idx < N) {
            a[idx] = p[idx];
        }
    }

    void math21_cuda_hello_world() {
        char *a_h, *a_d;
        const int N = 12;
        size_t size = N * sizeof(char);
        math21_memory_malloc((void **) &a_h, size);
        math21_cuda_malloc_device((void **) &a_d, size);
        for (int i = 0; i < N; i++) {
            a_h[i] = 0;
        }
        math21_cuda_memcpy_host_to_device(a_d, a_h, size);
        int blocksize = 4;
        int nblock = (N - 1 + blocksize) / blocksize;
        _print_hello_world << < nblock, blocksize >> > (a_d, N);
        math21_cuda_memcpy_device_to_host(a_h, a_d, sizeof(char) * N);
        printf("%s", a_h);
        math21_memory_free(a_h);
        math21_cuda_free_device(a_d);
    }

    void math21_cuda_thrust_version() {
        int major = THRUST_MAJOR_VERSION;
        int minor = THRUST_MINOR_VERSION;

        std::cout << "Thrust v" << major << "." << minor << std::endl;
    }

}