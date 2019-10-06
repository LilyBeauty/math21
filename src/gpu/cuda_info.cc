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
#include "cuda_info.h"

namespace math21 {
    namespace cuda {

        int getCudaEnabledDeviceCount() {
#ifndef MATH21_FLAG_USE_CUDA
            return 0;
#else
            int count;
            cudaError_t error = cudaGetDeviceCount(&count);

            if (error == cudaErrorInsufficientDriver)
                return -1;

            if (error == cudaErrorNoDevice)
                return 0;

            MATH21_ASSERT_CUDA_CALL(error);
            return count;
#endif
        }

        void setDevice(int device) {
#ifndef MATH21_FLAG_USE_CUDA
            math21_cuda_throw_no_cuda();
#else
            MATH21_ASSERT_CUDA_CALL(cudaSetDevice(device));
            MATH21_ASSERT_CUDA_CALL(math21_cuda_Free(0));
#endif
        }

        int getDevice() {
#ifndef MATH21_FLAG_USE_CUDA
            math21_cuda_throw_no_cuda();
            return 0;
#else
            int device;
            MATH21_ASSERT_CUDA_CALL(cudaGetDevice(&device));
            return device;
#endif
        }

        void resetDevice() {
#ifndef MATH21_FLAG_USE_CUDA
            math21_cuda_throw_no_cuda();
#else
            MATH21_ASSERT_CUDA_CALL(cudaDeviceReset());
#endif
        }

    }
}

