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
#include "tool.h"
#include "../memory/files.h"

namespace math21 {
#ifdef MATH21_WINDOWS
#include <windows.h>
    float getticks() {
        static double freq = -1.0;
        LARGE_INTEGER lint;

        if (freq < 0.0) {
            if (!QueryPerformanceFrequency(&lint))
                return -1.0f;

            freq = lint.QuadPart;
        }

        if (!QueryPerformanceCounter(&lint))
            return -1.0f;
        return (float) (lint.QuadPart / freq);
    }
#else

#include <time.h>

    float getticks() {
        struct timespec ts;

        if (clock_gettime(CLOCK_MONOTONIC, &ts) < 0)
            return -1.0f;

        return (float) (ts.tv_sec + 1e-9f * ts.tv_nsec);
    }

#endif

    NumR math21_time_getticks_no_parallel() {
        return getticks();
    }

    NumR math21_time_getticks() {
#ifdef MATH21_FLAG_IS_PARALLEL
#ifdef MATH21_FLAG_USE_OPENMP
        return omp_get_wtime();
#else
        return getticks();
#endif
#else
        return getticks();
#endif
    }

///////////////////////////////// timer ////////////////////////////////////

    timer::timer() {

    }

    timer::~timer() {
    }

    void timer::start() {
#ifdef MATH21_FLAG_USE_CUDA
        math21_cuda_EventCreate(&_start);
        math21_cuda_EventCreate(&_stop);
        math21_cuda_EventRecord(_start, 0);
#else
        t = math21_time_getticks();
#endif
    }

    void timer::end() {
#ifdef MATH21_FLAG_USE_CUDA
        math21_cuda_EventRecord(_stop, 0);
        math21_cuda_EventSynchronize(_stop);
        math21_cuda_EventElapsedTime(&elapsed_time, _start, _stop);
#else
        t = math21_time_getticks() - t;
#endif
    }

    NumR timer::time() {
#ifdef MATH21_FLAG_USE_CUDA
        return (NumR) elapsed_time;
#else
        return t*1000;
#endif
    }

}