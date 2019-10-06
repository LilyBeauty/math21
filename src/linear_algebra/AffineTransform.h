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

    void math21_la_2d_matrix_translate(NumR x, NumR y, MatR &T);

    void math21_la_2d_matrix_translate_reverse_mode(NumR x, NumR y, MatR &T);

    void math21_la_2d_matrix_scale(NumR x, NumR y, MatR &T);

    void math21_la_2d_matrix_scale_reverse_mode(NumR x, NumR y, MatR &T);

    void math21_la_2d_matrix_rotate(NumR theta, MatR &T);

    void math21_la_2d_matrix_rotate_reverse_mode(NumR theta, MatR &T);

    // shear x along x-axis, y along y-axis
    void math21_la_2d_matrix_shearing(NumR x, NumR y, MatR &T);

    // shear x along x-axis, y along y-axis
    void math21_la_2d_matrix_shearing_reverse_mode(NumR x, NumR y, MatR &T);

    void math21_la_2d_matrix_test(const TenR &A, TenR &B);

    void math21_la_3d_matrix_translate(NumR x, NumR y, NumR z, MatR &T);

    void math21_la_3d_matrix_translate_reverse_mode(NumR x, NumR y, NumR z, MatR &T);

    void math21_la_3d_matrix_scale(NumR x, NumR y, NumR z, MatR &T);

    void math21_la_3d_matrix_scale_reverse_mode(NumR x, NumR y, NumR z, MatR &T);

    void math21_la_3d_matrix_rotate_about_x_axis(NumR theta, MatR &T);

    void math21_la_3d_matrix_rotate_about_y_axis(NumR theta, MatR &T);

    void math21_la_3d_matrix_rotate_about_z_axis(NumR theta, MatR &T);

    // deprecate! use the one having math21 prefix instead.
    //! returns 2x3 affine transformation matrix for the planar rotation. angle is in radian
    void getRotationMatrix2D(const VecR &center, NumR angle, MatR &m);

    //! returns 3x3 affine transformation matrix for the planar rotation. angle is in radian
    //! coordinate axes are fixed.
    void math21_la_getRotationMatrix2D(const VecR &center, NumR angle, MatR &m);

    // rotate point p by angle and then translate by b.
    void math21_la_rotate_and_translate_point(const VecR &b, NumR angle, MatR &T);

    //! translate axis to b and then rotate by angle.
    //! T_inv is axis representation of new axis in old coordinate system.
    //! y = Ax+b, x is new coordinate of point p, y is old representation of p.
    //! T_inv = (A, b; 0, 1)
    void math21_la_translate_and_rotate_axis_reverse_mode(const VecR &b, NumR angle, MatR &T_inv);

    void math21_la_rotate_axis(NumR angle, MatR &T);

    void math21_la_3d_rotate_axis_about_x(NumR angle, MatR &T);

    void math21_la_3d_rotate_axis_about_y(NumR angle, MatR &T);

    void math21_la_3d_rotate_axis_about_z(NumR angle, MatR &T);

    void math21_la_3d_post_rotate_axis_about_x(NumR theta, MatR &T);

    void math21_la_3d_post_rotate_axis_about_y(NumR theta, MatR &T);

    void math21_la_3d_post_rotate_axis_about_z(NumR theta, MatR &T);

    //! (i, j) of B = T * (i, j) of A,
    //! T is affine transformation. y~ = T * x~
    //! T = (A, b; 0, 1), A is linear transformation, y = Ax+b,
    void math21_la_affine_transform_image(const MatR &A, MatR &B, const MatR &T);

    void math21_la_2d_affine_transform_image(const MatR &A, MatR &B, const MatR &T);

    void math21_la_2d_affine_transform_image_reverse_mode(const MatR &A, MatR &B, const MatR &T);

    void math21_la_3d_affine_transform_image(const MatR &A, MatR &B, const MatR &T);

    //! (i, j) of A = T * (i, j) of B
    void math21_la_affine_transform_image_reverse_mode(const MatR &A, MatR &B, const MatR &T);

    void math21_la_3d_affine_transform_image_reverse_mode(const MatR &A, MatR &B, const MatR &T);

    //!
    void math21_la_perspective_transform_image(const MatR &A, MatR &B, const MatR &T);

    // X = lambda * T * x; lambda is set to make sure X(3) = 1. Here T is 3*3.
    NumN geometry_project(const MatR &T, const VecR &x, VecR &X);

    // xs: {*,*,1;*,*,1;*,*,1}
    NumB geometry_project_in_batch(const MatR &T, const MatR &xs, MatR &Xs);

    // Xab = Xb - Xa. Here T is 3*3.
    NumN geometry_project(const MatR &T, const VecR &xa, const VecR &xb, VecR &Xab);

    NumB geometry_cal_projection(const VecR &x0, const VecR &x1, const VecR &x2, const VecR &x3, MatR &T);

    NumN geometry_cal_projection_scale(const VecR &xa, const VecR &xb, NumR l, MatR &T);

    NumB geometry_cal_projection_scale(const Seqce <VecR> &xas, const Seqce <VecR> &xbs,
                                       const Seqce <NumR> &ls,
                                       MatR &T);

    NumB geometry_cal_projection_to_world(const MatR &Xs, const MatR &xs, MatR &T);

    NumB geometry_project_image(const MatR &T, NumN nr, NumN nc, TenR &index);

    NumB geometry_project_image(const MatR &T, NumR x1, NumR x2, NumR y1, NumR y2, NumN nr, NumN nc, TenR &src_index);

    // X = T*x, xs * T_trans = Xs
    NumB geometry_cal_affine(const MatR &xs, const MatR &Xs, MatR &T);

    // (x1,y1), (x2, y2) as diagonal.
    NumB geometry_cal_affine_matrix_axis_to_matrix(MatR &T, NumR x1, NumR x2, NumR y1, NumR y2, NumN nr, NumN nc);

    void scale(const MatR &m, MatR &C, NumR a, NumR b, NumR c, NumR d);

    void scale(const TenR &X, TenR &Y, NumR c, NumR d);

    void sample_sine_voice(TenR &X, TenR &Y, NumN seconds);

    void math21_sample_heart(MatR &A);

    void draw_data_at_matrix_axis(const TenR &X_sample, const TenR &Y_sample,
                                  TenN &matrix_axis, NumN axis_x, NumN axis_y);

    // la means linear algebra.
    void la_scale_data_2d(MatR &data,
                          const MatR &M);

    void la_scale_data_2d(const MatR &data, MatR &data_new,
                          const MatR &M);

    void la_scale_data_2d(const MatR &data, MatR &data_new,
                          NumR c1, NumR d1, NumR c2, NumR d2);

    // deprecate, use math21_la_data_2d_bound instead.
    void la_data_2d_bound(const MatR &A,
                          NumR &a1, NumR &b1, NumR &a2, NumR &b2);

    void math21_la_data_2d_bound(const TenR &A,
                                 NumR &a1, NumR &b1, NumR &a2, NumR &b2);

    void math21_la_data_2d_bound(const TenR &A, Interval2D &I);

    void math21_la_data_2d_bound_in_batch(const TenR &A,
                                          NumR &a1, NumR &b1, NumR &a2, NumR &b2);

    void math21_la_data_2d_bound_in_batch(const TenR &A,
                                          Interval2D &I);

    void la_getScaleTranslationMatrix2D(MatR &M,
                                        NumR a1, NumR b1, NumR a2, NumR b2,
                                        NumR c1, NumR d1, NumR c2, NumR d2);

    void math21_la_getScaleTranslationMatrix2D(MatR &M,
                                               const Interval2D &I1,
                                               Interval2D &I2);

    void math21_la_2d_matrix_compute_matrix_axis_to_matrix(NumN axis_x, NumN axis_y, MatR &T);

    void math21_la_2d_matrix_compute_matrix_to_matrix_axis(NumN axis_x, NumN axis_y, MatR &T_final);

    void la_draw_data_2d_at_matrix_axis_no_scale(const MatR &data, TenN &matrix_axis);
}