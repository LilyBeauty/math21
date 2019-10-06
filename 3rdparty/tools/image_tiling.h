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

#include <math.h>
#include <cstdio>

struct tiling_config {
public:
    int m1, n1, m2, n2;
    int mk_min, nk_min;
    float alpha_m, alpha_n;
    bool noRedundant;

    tiling_config() {
        alpha_m = 0.1;
        alpha_n = 0.1;
        m1 = 2000;
        n1 = 1000;
        m2 = 5;
        n2 = 4;
        mk_min = 300;
        nk_min = 300;
        noRedundant = true;
    }

    virtual ~tiling_config() {
    }
};

// results should have size at least m2*n2+2 provided by caller.
int tiling(const tiling_config *pconfig, int *results);

int test_tiling();