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

#include "image.h"
#include "../matrix_op/files.h"
#include "../functions/files.h"

namespace math21 {
    //r, c are dst size. e.x., (0, 100] -> (0, 1000].
    void math21_img_resize_method_sampling(const MatR &src, MatR &dst) {
        MATH21_ASSERT(!src.isEmpty(), "src matrix is empty!");
        MATH21_ASSERT(!dst.isEmpty(), "dst matrix is empty!");
        MATH21_ASSERT((src.dims() == 2 && dst.dims() == 2) ||
                      (src.dims() == 3 && dst.dims() == 3 && src.dim(1) == dst.dim(1)),
                      "" << src.shape().log("src")
                         << dst.shape().log("dst")
        );
        NumN r, c;
        NumN src_r, src_c;
        NumN nch;
        if (src.dims() == 2) {
            r = dst.dim(1);
            c = dst.dim(2);
            src_r = src.dim(1);
            src_c = src.dim(2);
        } else {
            r = dst.dim(2);
            c = dst.dim(3);
            src_r = src.dim(2);
            src_c = src.dim(3);
            nch = src.dim(1);
        }
        NumN i, j, k;
        NumN src_i, src_j;

        NumR rs, cs;
        rs = src_r / (NumR) r;
        cs = src_c / (NumR) c;
        if (src.dims() == 2) {
            for (i = 1; i <= r; i++) {
                src_i = (NumN) (i * rs);
                math21_clip_not_less(src_i, 1);
                for (j = 1; j <= c; j++) {
                    src_j = (NumN) (j * cs);
                    math21_clip_not_less(src_j, 1);
                    dst(i, j) = src(src_i, src_j);
                }
            }
        } else {
            for (i = 1; i <= r; i++) {
                src_i = (NumN) (i * rs);
                math21_clip_not_less(src_i, 1);
                for (j = 1; j <= c; j++) {
                    src_j = (NumN) (j * cs);
                    math21_clip_not_less(src_j, 1);
                    for (k = 1; k <= nch; ++k) {
                        dst(k, i, j) = src(k, src_i, src_j);
                    }
                }
            }
        }

    }

    // we use max pooling
    void math21_img_resize_method_pooling(const MatR &src, MatR &dst) {
        MATH21_ASSERT(!src.isEmpty(), "src matrix is empty!");
        MATH21_ASSERT(!dst.isEmpty(), "dst matrix is empty!");
        MATH21_ASSERT((src.dims() == 2 && dst.dims() == 2) ||
                      (src.dims() == 3 && dst.dims() == 3 && src.dim(1) == dst.dim(1)),
                      "" << src.shape().log("src")
                         << dst.shape().log("dst"));
        NumN r, c;
        NumN src_r, src_c;
        if (src.dims() == 2) {
            r = dst.dim(1);
            c = dst.dim(2);
            src_r = src.dim(1);
            src_c = src.dim(2);
        } else {
            r = dst.dim(2);
            c = dst.dim(3);
            src_r = src.dim(2);
            src_c = src.dim(3);
        }

        NumN mk, nk, ms, ns;
        math21_operator_ml_pooling_get_mk_ms(src_r, src_c, r, c, mk, nk, ms, ns);
        if (src.dims() == 2) {
            TenR src_share, dst_share;
            math21_operator_shareReshape_2d_to_3d(src, src_share);
            math21_operator_shareReshape_2d_to_3d(dst, dst_share);
            math21_operator_ml_pooling_valueAt(src_share, dst_share, cnn_type_pooling_max,
                                               mk, nk, ms, ns);

        } else {
            math21_operator_ml_pooling_valueAt(src, dst, cnn_type_pooling_max,
                                               mk, nk, ms, ns);

        }
    }

    void math21_img_resize(const MatR &src, MatR &dst, NumN img_resize_method) {
        MATH21_ASSERT(!src.isEmpty(), "src matrix is empty!");
        MATH21_ASSERT(!dst.isEmpty(), "dst matrix is empty!");
        MATH21_ASSERT((src.dims() == 2 && dst.dims() == 2) ||
                      (src.dims() == 3 && dst.dims() == 3 && src.dim(1) == dst.dim(1)),
                      "" << src.shape().log("src")
                         << dst.shape().log("dst")
        );
        NumN r, c;
        NumN src_r, src_c;
        if (src.dims() == 2) {
            r = dst.dim(1);
            c = dst.dim(2);
            src_r = src.dim(1);
            src_c = src.dim(2);
        } else {
            r = dst.dim(2);
            c = dst.dim(3);
            src_r = src.dim(2);
            src_c = src.dim(3);
        }

        if (img_resize_method == img_resize_method_default) {
            if (r <= src_r && c <= src_c) {
                math21_img_resize_method_pooling(src, dst);
            } else {
                math21_img_resize_method_sampling(src, dst);
            }
        } else if (img_resize_method == img_resize_method_sampling) {
            math21_img_resize_method_sampling(src, dst);
        } else if (img_resize_method == img_resize_method_pooling) {
            math21_img_resize_method_pooling(src, dst);
        }
    }

    void math21_img_resize(const Seqce<TenR> &srcs, Seqce<TenR> &dsts, NumN img_resize_method) {
        MATH21_ASSERT(srcs.size() == dsts.size())
        for (NumN i = 1; i <= srcs.size(); i++) {
            const TenR &src = srcs(i);
            TenR &dst = dsts.at(i);
            math21_img_resize(src, dst, img_resize_method);
        }
    }

    // just return if image already has size d.
    void math21_img_resize(Seqce<TenR> &images, const VecN &d, NumN img_resize_method) {
        for (NumN i = 1; i <= images.size(); i++) {
            TenR &x = images.at(i);
            math21_img_resize(x, d, img_resize_method);
        }
    }

    void math21_img_resize(TenR &image, const VecN &d, NumN img_resize_method) {
        if (image.isSameSize(d)) {
            return;
        }
        TenR tmp;
        tmp.setSize(d);
        math21_img_resize(image, tmp, img_resize_method);
        image.swap(tmp);
    }

    // convert all color images to gray images. Just return if image is already gray.
    void math21_img_rgb_to_gray(Seqce<TenR> &images) {
        for (NumN i = 1; i <= images.size(); i++) {
            TenR &x = images.at(i);
            math21_img_rgb_to_gray(x);
        }
    }

    // argb, rgb, etc.
    void math21_img_rgb_to_gray(MatR &image) {
        if (image.dims() == 2) {
            return;
        }
        MATH21_ASSERT(image.dims() == 3)
        TenR tmp;
        tmp.setSize(image.dim(2), image.dim(3));
        math21_img_rgb_to_gray(image, tmp);
        image.swap(tmp);
    }

    void math21_img_gray_to_binary(MatR &image) {
        MATH21_ASSERT(image.dims() == 2)
        TenR tmp;
        tmp.setSize(image.shape());
        math21_img_gray_to_binary(image, tmp);
        image.swap(tmp);
    }

    void math21_img_rgb_to_gray(const MatR &src, MatR &dst) {
        MATH21_ASSERT(!src.isEmpty(), "src matrix is empty!");
        MATH21_ASSERT(!dst.isEmpty(), "dst matrix is empty!");
        MATH21_ASSERT(src.dims() == 3)
        MATH21_ASSERT(dst.dims() == 2 && dst.isSameSize(src.dim(2), src.dim(3)));
        NumN r, c;
        NumN nch;
        r = dst.dim(1);
        c = dst.dim(2);
        nch = src.dim(1);

        NumN i, j, k;
        NumR sum;
        for (i = 1; i <= r; i++) {
            for (j = 1; j <= c; j++) {
                sum = 0;
                for (k = 1; k <= nch; ++k) {
                    sum += src(k, i, j);
                }
                dst(i, j) = sum / nch;
            }
        }
    }

    // histogram has shape: nch*256
    void math21_img_histogram(const MatR &src, MatR &histogram) {
        MATH21_ASSERT(!src.isEmpty(), "src matrix is empty!");
        MATH21_ASSERT(src.dims() == 3 || src.dims() == 2)
        NumN r, c;
        NumN nch;
        if (src.dims() == 3) {
            r = src.dim(2);
            c = src.dim(3);
            nch = src.dim(1);
        } else {
            nch = 1;
            r = src.dim(1);
            c = src.dim(2);
        }
        if (histogram.isEmpty()) {
            histogram.setSize(nch, 256);
        }
        if (!histogram.isEmpty()) {
            MATH21_ASSERT(histogram.isSameSize(nch, 256));
        }

        NumN i, j, k;
        NumN index;
        if (nch > 1) {
            for (i = 1; i <= r; i++) {
                for (j = 1; j <= c; j++) {
                    for (k = 1; k <= nch; ++k) {
                        index = (NumN) src(k, i, j) + 1;
                        ++histogram(k, index);
                    }
                }
            }
        } else {
            for (i = 1; i <= r; i++) {
                for (j = 1; j <= c; j++) {
                    index = (NumN) src(i, j) + 1;
                    ++histogram(1, index);
                }
            }
        }

    }

    void math21_img_cluster_by_value(const MatR &src, MatN &mask) {

    }

    // dst is mask with values in {1, 2, ...}
    void math21_img_gray_cluster_by_value(const MatR &src, MatN &dst, NumN K, VecN &num_in_clusters) {
        MATH21_ASSERT(!src.isEmpty(), "src matrix is empty!");
        MATH21_ASSERT(!dst.isEmpty(), "dst matrix is empty!");
        MATH21_ASSERT(src.dims() == 2)
        MATH21_ASSERT(dst.dims() == 2 && dst.isSameSize(src.shape()));
        NumN total_points = src.volume();

        VecN labels;
        VecR tmp_src;
        math21_operator_shareReshape_to_vector(dst, labels);
        math21_operator_shareReshape_to_vector(src, tmp_src);

        Seqce<TenR> data;
        math21_tool_vec_2_seqce(tmp_src, data);
        ml_kmeans_config config(K, total_points, 1, 100);
        ml_kmeans(data, labels, num_in_clusters, config);
    }

    // background is 0, foreground is 255
    void math21_img_gray_to_binary(const MatR &src, MatR &dst) {
        MATH21_ASSERT(!src.isEmpty(), "src matrix is empty!");
        MATH21_ASSERT(!dst.isEmpty(), "dst matrix is empty!");
        MATH21_ASSERT(src.dims() == 2)
        MATH21_ASSERT(dst.dims() == 2 && dst.isSameSize(src.shape()));

        MatN mask;
        mask.setSize(src.shape());
        VecN num_in_clusters;
        math21_img_gray_cluster_by_value(src, mask, 2, num_in_clusters);
        NumN r, c;
        r = dst.dim(1);
        c = dst.dim(2);

        NumN bg_index;
        if (num_in_clusters(1) > num_in_clusters(2)) {
            bg_index = 1;
        } else {
            bg_index = 2;
        }
        NumN i, j;
        for (i = 1; i <= r; i++) {
            for (j = 1; j <= c; j++) {
                if (mask(i, j) == bg_index) {
                    dst(i, j) = 0;
                } else {
                    dst(i, j) = 255;
                }
            }
        }
    }

    void math21_img_gray_to_binary(Seqce<TenR> &images) {
        for (NumN i = 1; i <= images.size(); i++) {
            TenR &x = images.at(i);
            math21_img_gray_to_binary(x);
        }
    }

}