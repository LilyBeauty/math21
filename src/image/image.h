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

    struct ImageDraw {

        // deprecate
        // draw and save
        virtual void draw(const TenR &A, const char *name, NumB isBatch = 1) {
            MATH21_ASSERT(0, "Please override to use!");
        }

        virtual void setDir(const char *name) {
            MATH21_ASSERT(0, "Please override to use!");
        }

        // reset image to bg color.
        virtual void reset() {
            MATH21_ASSERT(0, "Please override to use!");
        }

        // don't save
        virtual void plot(const TenR &A, const char *name, NumB isBatch = 1) {
            MATH21_ASSERT(0, "Please override to use!");
        }

        virtual void save(const char *name) {
            MATH21_ASSERT(0, "Please override to use!");
        }
    };

    struct ImageDraw_Dummy : public ImageDraw {
        ImageDraw_Dummy() = default;

        void draw(const TenR &A, const char *name, NumB isBatch = 1) override {
            m21log("ImageDraw_Dummy draw!");
        }

        void setDir(const char *name) override {
            m21log("ImageDraw_Dummy setDir!");
        }

        void reset() override {
            m21log("ImageDraw_Dummy reset!");
        }

        // don't save
        void plot(const TenR &A, const char *name, NumB isBatch = 1) override {
            m21log("ImageDraw_Dummy plot!");
        }

        void save(const char *name) override {
            m21log("ImageDraw_Dummy save!");
        }
    };


    enum {
        img_resize_method_default = 1,
        img_resize_method_sampling,
        img_resize_method_pooling,//
    };

    void math21_img_resize(const MatR &src, MatR &dst, NumN img_resize_method = img_resize_method_default);

    void math21_img_resize(const Seqce <TenR> &srcs, Seqce <TenR> &dsts,
                           NumN img_resize_method = img_resize_method_default);

    void math21_img_resize(Seqce <TenR> &images, const VecN &d, NumN img_resize_method = img_resize_method_default);

    void math21_img_resize(TenR &image, const VecN &d, NumN img_resize_method = img_resize_method_default);

    void math21_img_rgb_to_gray(Seqce <TenR> &images);

    void math21_img_rgb_to_gray(MatR &image);

    void math21_img_rgb_to_gray(const MatR &src, MatR &dst);

    void math21_img_gray_cluster_by_value(const MatR &src, MatN &dst, NumN K, VecN &num_in_clusters);

    void math21_img_gray_to_binary(MatR &image);

    void math21_img_gray_to_binary(const MatR &src, MatR &dst);

    // slow, so use for small image.
    void math21_img_gray_to_binary(Seqce <TenR> &images);

    void math21_img_histogram(const MatR &src, MatR &histogram);

    // image nch*nr*nc -> nr*nc*nch
    template<typename T, typename S>
    void math21_img_planar_to_interleaved(const Tensor <T> &A, Tensor <S> &B) {
        MATH21_ASSERT(A.dims() == 3)
        NumN nr, nc, nch;
        NumN i1, i2, i3;
        nch = A.dim(1);
        nr = A.dim(2);
        nc = A.dim(3);
        if (A.dims() == 3) {
            MATH21_ASSERT(nch == 1 || nch == 3 || nch == 4,
                          "not 1, 3 or 4 channels, channels: " << nch);
        }
        if (B.isSameSize(nr, nc, nch) == 0) {
            B.setSize(nr, nc, nch);
        }
        T tmp;
        if (typeid(S) == typeid(NumN8)) {
            for (i1 = 1; i1 <= nch; ++i1) {
                for (i2 = 1; i2 <= nr; ++i2) {
                    for (i3 = 1; i3 <= nc; ++i3) {
                        tmp = A(i1, i2, i3);
                        B(i2, i3, i1) = (NumN8) (tmp < 0 ? 0 :
                                                 (tmp > 255 ? 255 : tmp));
                    }
                }
            }
        } else {
            for (i1 = 1; i1 <= nch; ++i1) {
                for (i2 = 1; i2 <= nr; ++i2) {
                    for (i3 = 1; i3 <= nc; ++i3) {
                        B(i2, i3, i1) = A(i1, i2, i3);
                    }
                }
            }
        }

    }

    // image nr*nc*nch -> nch*nr*nc
    template<typename T, typename S>
    void math21_img_interleaved_to_planar(const Tensor <T> &A, Tensor <S> &B) {
        MATH21_ASSERT(A.dims() == 3)
        NumN nr, nc, nch;
        NumN i1, i2, i3;
        nch = A.dim(3);
        nr = A.dim(1);
        nc = A.dim(2);
        if (A.dims() == 3) {
            MATH21_ASSERT(nch == 1 || nch == 3 || nch == 4,
                          "not 1, 3 or 4 channels, channels: " << nch);
        }
        if (B.isSameSize(nch, nr, nc) == 0) {
            B.setSize(nch, nr, nc);
        }
        T tmp;
        if (typeid(S) == typeid(NumN8)) {
            for (i1 = 1; i1 <= nr; ++i1) {
                for (i2 = 1; i2 <= nc; ++i2) {
                    for (i3 = 1; i3 <= nch; ++i3) {
                        tmp = A(i1, i2, i3);
                        B(i3, i1, i2) = (NumN8) (tmp < 0 ? 0 :
                                                 (tmp > 255 ? 255 : tmp));
                    }
                }
            }
        } else {
            for (i1 = 1; i1 <= nr; ++i1) {
                for (i2 = 1; i2 <= nc; ++i2) {
                    for (i3 = 1; i3 <= nch; ++i3) {
                        B(i3, i1, i2) = A(i1, i2, i3);
                    }
                }
            }
        }

    }

}