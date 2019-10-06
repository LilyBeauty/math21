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

#include "../matrix_op/files.h"
#include "../functions/files.h"
#include "AffineTransform.h"

namespace math21 {

    void math21_la_2d_matrix_translate(NumR x, NumR y, MatR &T) {
        if (!T.isSameSize(3, 3)) {
            T.setSize(3, 3);
        }
        T =
                1, 0, x,
                0, 1, y,
                0, 0, 1;
    }

    void math21_la_2d_matrix_translate_reverse_mode(NumR x, NumR y, MatR &T) {
        math21_la_2d_matrix_translate(-x, -y, T);
    }

    void math21_la_2d_matrix_scale(NumR x, NumR y, MatR &T) {
        MATH21_ASSERT(x != 0 && y != 0)
        if (!T.isSameSize(3, 3)) {
            T.setSize(3, 3);
        }
        T =
                x, 0, 0,
                0, y, 0,
                0, 0, 1;
    }

    void math21_la_2d_matrix_scale_reverse_mode(NumR x, NumR y, MatR &T) {
        MATH21_ASSERT(x != 0 && y != 0)
        math21_la_2d_matrix_scale(1 / x, 1 / y, T);
    }

    void math21_la_2d_matrix_rotate(NumR theta, MatR &T) {
        if (!T.isSameSize(3, 3)) {
            T.setSize(3, 3);
        }
        NumR a, b, c, d;
        a = xjcos(theta);
        b = xjsin(theta);
        c = -xjsin(theta);
        d = xjcos(theta);
        T =
                a, c, 0,
                b, d, 0,
                0, 0, 1;
    }

    void math21_la_2d_matrix_rotate_reverse_mode(NumR theta, MatR &T) {
        math21_la_2d_matrix_rotate(-theta, T);
    }

    // shear x along x-axis, y along y-axis
    void math21_la_2d_matrix_shearing(NumR x, NumR y, MatR &T) {
        MATH21_ASSERT(x * y != 1)
        if (!T.isSameSize(3, 3)) {
            T.setSize(3, 3);
        }

        T =
                1, x, 0,
                y, 1, 0,
                0, 0, 1;
    }

// shear x along x-axis, y along y-axis
    void math21_la_2d_matrix_shearing_reverse_mode(NumR x, NumR y, MatR &T) {
        MATH21_ASSERT(x * y != 1)
        if (!T.isSameSize(3, 3)) {
            T.setSize(3, 3);
        }
        NumR a, b, c, d, det;
        det = 1 - x * y;
        a = 1 / det;
        b = -y / det;
        c = -x / det;
        d = 1 / det;
        T =
                a, c, 0,
                b, d, 0,
                0, 0, 1;
    }

    void math21_la_2d_matrix_test(const TenR &A, TenR &B) {
        // get matrix
        MatR T_final, T;
        T_final.setSize(3, 3);
        math21_operator_mat_eye(T_final);

        math21_la_2d_matrix_scale(0.5, 0.5, T);
        math21_operator_multiply_to_B(1, T, T_final);
        math21_la_2d_matrix_rotate(MATH21_PI / 6, T);
        math21_operator_multiply_to_B(1, T, T_final);
        math21_la_2d_matrix_shearing(0.5, 0.2, T);
        math21_operator_multiply_to_B(1, T, T_final);
        math21_la_2d_matrix_translate(50, 0, T);
        math21_operator_multiply_to_B(1, T, T_final);

        // transform
        math21_la_2d_affine_transform_image(A, B, T_final);
    }

    void math21_la_3d_matrix_translate(NumR x, NumR y, NumR z, MatR &T) {
        if (!T.isSameSize(4, 4)) {
            T.setSize(4, 4);
        }
        T =
                1, 0, 0, x,
                0, 1, 0, y,
                0, 0, 1, z,
                0, 0, 0, 1;
    }

    void math21_la_3d_matrix_translate_reverse_mode(NumR x, NumR y, NumR z, MatR &T) {
        math21_la_3d_matrix_translate(-x, -y, -z, T);
    }

    void math21_la_3d_matrix_scale(NumR x, NumR y, NumR z, MatR &T) {
        MATH21_ASSERT(x != 0 && y != 0 && z != 0)
        if (!T.isSameSize(4, 4)) {
            T.setSize(4, 4);
        }
        T =
                x, 0, 0, 0,
                0, y, 0, 0,
                0, 0, z, 0,
                0, 0, 0, 1;
    }

    void math21_la_3d_matrix_scale_reverse_mode(NumR x, NumR y, NumR z, MatR &T) {
        MATH21_ASSERT(x != 0 && y != 0 && z != 0)
        math21_la_3d_matrix_scale(1 / x, 1 / y, 1 / z, T);
    }


    void math21_la_3d_matrix_rotate_about_x_axis(NumR theta, MatR &T) {
        if (!T.isSameSize(4, 4)) {
            T.setSize(4, 4);
        }
        NumR a, b, c, d;
        a = xjcos(theta);
        b = xjsin(theta);
        c = -xjsin(theta);
        d = xjcos(theta);
        T =
                1, 0, 0, 0,
                0, a, c, 0,
                0, b, d, 0,
                0, 0, 0, 1;
    }

    void math21_la_3d_matrix_rotate_about_y_axis(NumR theta, MatR &T) {
        if (!T.isSameSize(4, 4)) {
            T.setSize(4, 4);
        }
        NumR a, b, c, d;
        a = xjcos(theta);
        b = xjsin(theta);
        c = -xjsin(theta);
        d = xjcos(theta);
        T =
                d, 0, b, 0,
                0, 1, 0, 0,
                c, 0, a, 0,
                0, 0, 0, 1;
    }

    void math21_la_3d_matrix_rotate_about_z_axis(NumR theta, MatR &T) {
        if (!T.isSameSize(4, 4)) {
            T.setSize(4, 4);
        }
        NumR a, b, c, d;
        a = xjcos(theta);
        b = xjsin(theta);
        c = -xjsin(theta);
        d = xjcos(theta);
        T =
                a, c, 0, 0,
                b, d, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1;
    }

    //! returns 2x3 affine transformation matrix for the planar rotation.
    void getRotationMatrix2D(const VecR &x0, NumR theta, MatR &B) {
        MATH21_ASSERT(x0.nrows() == 2, "center is not in plane.");
        if (!B.isSameSize(2, 3)) {
            B.setSize(2, 3);
        }
        MatR b(2, 1), A(2, 2);
        A =
                xjcos(theta), -xjsin(theta),
                xjsin(theta), xjcos(theta);
        math21_operator_multiply(1, A, x0, b);
        math21_operator_linear_to_B(1.0, x0, -1.0, b);
//        b = x0 - A * x0;
        B =
                A(1, 1), A(1, 2), b(1, 1),
                A(2, 1), A(2, 2), b(2, 1);
    }

    // y = A(x-x0) + x0, so y_tilde = B * x_tilde
    void math21_la_getRotationMatrix2D(const VecR &x0, NumR theta, MatR &B) {
        MATH21_ASSERT(x0.nrows() == 2, "center is not in plane.");
        if (!B.isSameSize(3, 3)) {
            B.setSize(3, 3);
        }
        MatR b(2, 1), A(2, 2);
        A =
                xjcos(theta), -xjsin(theta),
                xjsin(theta), xjcos(theta);
        math21_operator_multiply(1, A, x0, b);
        math21_operator_linear_to_B(1.0, x0, -1.0, b);
//        b = x0 - A * x0;
        B =
                A(1, 1), A(1, 2), b(1, 1),
                A(2, 1), A(2, 2), b(2, 1),
                0, 0, 1;
    }

    void math21_la_rotate_and_translate_point(const VecR &b, NumR theta, MatR &T) {
        MATH21_ASSERT(b.nrows() == 2);
        if (!T.isSameSize(3, 3)) {
            T.setSize(3, 3);
        }
        MatR A(2, 2);
        A =
                xjcos(theta), -xjsin(theta),
                xjsin(theta), xjcos(theta);
        T =
                A(1, 1), A(1, 2), b(1, 1),
                A(2, 1), A(2, 2), b(2, 1),
                0, 0, 1;
    }

    void math21_la_translate_and_rotate_axis_reverse_mode(const VecR &b, NumR theta, MatR &T) {
        math21_la_rotate_and_translate_point(b, theta, T);
    }

    void math21_la_rotate_axis(NumR theta, MatR &T) {
        if (!T.isSameSize(3, 3)) {
            T.setSize(3, 3);
        }
        MatR A(2, 2);
        A =
                xjcos(theta), xjsin(theta),
                -xjsin(theta), xjcos(theta);
        T =
                A(1, 1), A(1, 2), 0,
                A(2, 1), A(2, 2), 0,
                0, 0, 1;
    }

    void math21_la_3d_rotate_axis_about_x(NumR theta, MatR &T) {
        if (!T.isSameSize(4, 4)) {
            T.setSize(4, 4);
        }
        NumR a, b, c, d;
        a = xjcos(theta);
        b = -xjsin(theta);
        c = xjsin(theta);
        d = xjcos(theta);
        T =
                1, 0, 0, 0,
                0, a, c, 0,
                0, b, d, 0,
                0, 0, 0, 1;
    }

    void math21_la_3d_rotate_axis_about_y(NumR theta, MatR &T) {
        if (!T.isSameSize(4, 4)) {
            T.setSize(4, 4);
        }
        NumR a, b, c, d;
        a = xjcos(theta);
        b = -xjsin(theta);
        c = xjsin(theta);
        d = xjcos(theta);
        T =
                d, 0, b, 0,
                0, 1, 0, 0,
                c, 0, a, 0,
                0, 0, 0, 1;
    }

    void math21_la_3d_rotate_axis_about_z(NumR theta, MatR &T) {
        if (!T.isSameSize(4, 4)) {
            T.setSize(4, 4);
        }
        NumR a, b, c, d;
        a = xjcos(theta);
        b = -xjsin(theta);
        c = xjsin(theta);
        d = xjcos(theta);
        T =
                a, c, 0, 0,
                b, d, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1;
    }

    void math21_la_3d_post_rotate_axis_about_x(NumR theta, MatR &T) {
        MATH21_ASSERT(T.isSameSize(4, 4))
        MatR R;
        math21_la_3d_rotate_axis_about_x(theta, R);
        math21_operator_multiply_to_B(1, R, T);
    }

    void math21_la_3d_post_rotate_axis_about_y(NumR theta, MatR &T) {
        MATH21_ASSERT(T.isSameSize(4, 4))
        MatR R;
        math21_la_3d_rotate_axis_about_y(theta, R);
        math21_operator_multiply_to_B(1, R, T);
    }

    void math21_la_3d_post_rotate_axis_about_z(NumR theta, MatR &T) {
        MATH21_ASSERT(T.isSameSize(4, 4))
        MatR R;
        math21_la_3d_rotate_axis_about_z(theta, R);
        math21_operator_multiply_to_B(1, R, T);
    }

    void math21_la_2d_affine_transform_image(const MatR &A, MatR &B, const MatR &T) {
        math21_la_affine_transform_image(A, B, T);
    }

    void math21_la_2d_affine_transform_image_reverse_mode(const MatR &A, MatR &B, const MatR &T) {
        math21_la_affine_transform_image_reverse_mode(A, B, T);
    }

    void math21_la_affine_transform_image(const MatR &A, MatR &B, const MatR &T) {
        MATH21_ASSERT(T.isSameSize(3, 3))
        MATH21_ASSERT(T(3, 1) == 0 && T(3, 2) == 0 && T(3, 3) == 1)
        MatR T_inv;
        math21_operator_inverse(T, T_inv);
        math21_la_affine_transform_image_reverse_mode(A, B, T_inv);
    }

    void math21_la_3d_affine_transform_image(const MatR &A, MatR &B, const MatR &T) {
        MATH21_ASSERT(T.isSameSize(4, 4))
        MATH21_ASSERT(math21_operator_num_isEqual(T(4, 1), 0, 1e-10)
                      && math21_operator_num_isEqual(T(4, 2), 0, 1e-10)
                      && math21_operator_num_isEqual(T(4, 3), 0, 1e-10)
                      && math21_operator_num_isEqual(T(4, 4), 1, 1e-10), "" << T.log("T"))
        MatR T_inv;
        math21_operator_inverse(T, T_inv);
        math21_la_3d_affine_transform_image_reverse_mode(A, B, T_inv);
    }

    void math21_la_affine_transform_image_reverse_mode(const MatR &A, MatR &B, const MatR &T) {
        MATH21_ASSERT(T.isSameSize(3, 3))
        MATH21_ASSERT(math21_operator_num_isEqual(T(3, 1), 0, 1e-10)
                      && math21_operator_num_isEqual(T(3, 2), 0, 1e-10)
                      && math21_operator_num_isEqual(T(3, 3), 1, 1e-10), "" << T.log("T"))
        if (B.isEmpty()) {
            B.setSize(A.shape());
        }
        MATH21_ASSERT(A.dims() == B.dims())
        MATH21_ASSERT(A.dims() == 3 || A.dims() == 2)
        NumN nch;
        NumN nr;
        NumN nc;
        NumN nr_A, nc_A;
        NumN dims = A.dims();
        if (A.dims() == 3) {
            MATH21_ASSERT(A.dim(1) == B.dim(1))
            nch = B.dim(1);
            nr = B.dim(2);
            nc = B.dim(3);
            nr_A = A.dim(2);
            nc_A = A.dim(3);
        } else {
            nr = B.dim(1);
            nc = B.dim(2);
            nch = 1;
            nr_A = A.dim(1);
            nc_A = A.dim(2);
        }

        NumN i1, i2, k;
        VecR x(3), y(3);
        x(3) = 1;
        NumN y1, y2;
        for (i1 = 1; i1 <= nr; ++i1) {
            for (i2 = 1; i2 <= nc; ++i2) {
                x(1) = i1;
                x(2) = i2;
                math21_operator_multiply(1, T, x, y);
                y1 = (NumN) y(1);
                y2 = (NumN) y(2);
                if (y1 >= 1 && y1 <= nr_A && y2 >= 1 && y2 <= nc_A) {
                    if (dims == 3) {
                        for (k = 1; k <= nch; ++k) {
                            B(k, i1, i2) = A(k, y1, y2);
                        }
                    } else {
                        B(i1, i2) = A(y1, y2);
                    }
                }
//                else {
//                    if (dims == 3) {
//                        for (k = 1; k <= nch; ++k) {
//                            B(k, i1, i2) = 0;
//                        }
//                    } else {
//                        B(i1, i2) = 0;
//                    }
//                }
            }
        }
    }

    void math21_la_3d_affine_transform_image_reverse_mode(const MatR &A, MatR &B, const MatR &T) {
        MATH21_ASSERT(T.isSameSize(4, 4))
        MATH21_ASSERT(math21_operator_num_isEqual(T(4, 1), 0, 1e-10)
                      && math21_operator_num_isEqual(T(4, 2), 0, 1e-10)
                      && math21_operator_num_isEqual(T(4, 3), 0, 1e-10)
                      && math21_operator_num_isEqual(T(4, 4), 1, 1e-10), "" << T.log("T"))
        if (B.isEmpty()) {
            B.setSize(A.shape());
        }
        MATH21_ASSERT(A.dims() == B.dims())
        MATH21_ASSERT(A.dims() == 3 || A.dims() == 2)
        NumN nch;
        NumN nr;
        NumN nc;
        NumN nr_A, nc_A;
        NumN dims = A.dims();
        if (A.dims() == 3) {
            MATH21_ASSERT(A.dim(1) == B.dim(1))
            nch = B.dim(1);
            nr = B.dim(2);
            nc = B.dim(3);
            nr_A = A.dim(2);
            nc_A = A.dim(3);
        } else {
            nr = B.dim(1);
            nc = B.dim(2);
            nch = 1;
            nr_A = A.dim(1);
            nc_A = A.dim(2);
        }

        NumN i1, i2, k;
        VecR x(4), y(4);
        x(3) = 0;
        x(4) = 1;
        NumN y1, y2;
        for (i1 = 1; i1 <= nr; ++i1) {
            for (i2 = 1; i2 <= nc; ++i2) {
                x(1) = i1;
                x(2) = i2;
                math21_operator_multiply(1, T, x, y);
                y1 = (NumN) y(1);
                y2 = (NumN) y(2);
                if (y1 >= 1 && y1 <= nr_A && y2 >= 1 && y2 <= nc_A) {
                    if (dims == 3) {
                        for (k = 1; k <= nch; ++k) {
                            B(k, i1, i2) = A(k, y1, y2);
                        }
                    } else {
                        B(i1, i2) = A(y1, y2);
                    }
                } else {
                    if (dims == 3) {
                        for (k = 1; k <= nch; ++k) {
                            B(k, i1, i2) = 0;
                        }
                    } else {
                        B(i1, i2) = 0;
                    }
                }
            }
        }
    }

    void math21_la_perspective_transform_image(const MatR &A, MatR &B, const MatR &T) {

    }

    ////! R should be orthogonal, but here may not be.
    NumB geometry_cal_projection(const VecR &_x0, const VecR &_x1, const VecR &_x2, const VecR &_x3, MatR &T) {
        MATH21_ASSERT(_x0.size() == 3 && _x1.size() == 3 && _x2.size() == 3 && _x3.size() == 3)
        MATH21_ASSERT(!math21_operator_isEqualZero(_x0) && !math21_operator_isEqualZero(_x1)
                      && !math21_operator_isEqualZero(_x2) && !math21_operator_isEqualZero(_x3));
        // pre-process
        VecR x0, x1, x2, x3;
        x0.setSize(_x0.shape());
        x0.assign(_x0);
        x1.setSize(_x1.shape());
        x1.assign(_x1);
        x2.setSize(_x2.shape());
        x2.assign(_x2);
        x3.setSize(_x3.shape());
        x3.assign(_x3);
        if (x0(3) != 0) {
            math21_operator_linear_to(1.0 / x0(3), x0);
            x0(3) = 0;
        }
        if (x1(3) != 0) {
            math21_operator_linear_to(1.0 / x1(3), x1);
            math21_operator_linear_to_A(1.0, x1, -1.0, x0);
        }
        if (x2(3) != 0) {
            math21_operator_linear_to(1.0 / x2(3), x2);
            math21_operator_linear_to_A(1.0, x2, -1.0, x0);
        }
        if (x3(3) != 0) {
            math21_operator_linear_to(1.0 / x3(3), x3);
            math21_operator_linear_to_A(1.0, x3, -1.0, x0);
        }
        NumR num = x1(3) + x2(3) + x3(3);
        if (num < 2.0) {
            m21error("no focus");
            return 0;
        }

        // calculate f
        NumR f12 = 0.0, f13 = 0.0, f23 = 0.0, f;
        NumR tmp;
        if (x1(3) != 0 && x2(3) != 0) {
            tmp = math21_operator_InnerProduct(1.0, x1, x2);
            tmp = tmp - 1;
            tmp = -tmp;
            if (tmp < 0) {// we give it a chance because we think it is accidently negative.
//                m21log("f12 negative");
                x2(1) = x2(1) * (-1);
                x2(2) = x2(2) * (-1);
                f12 = xjsqrt(-tmp);
            } else if (tmp > 0) {
                f12 = xjsqrt(tmp);
            }
        }
        if (x1(3) != 0 && x3(3) != 0) {
            tmp = math21_operator_InnerProduct(1.0, x1, x3);
            tmp = tmp - 1;
            tmp = -tmp;
            if (tmp > 0) {
                f13 = xjsqrt(tmp);
            }
        }
        if (x2(3) != 0 && x3(3) != 0) {
            tmp = math21_operator_InnerProduct(1.0, x2, x3);
            tmp = tmp - 1;
            tmp = -tmp;
            if (tmp > 0) {
                f23 = xjsqrt(tmp);
            }
        }
        m21log("\nf12", f12);
        m21log("f13", f13);
        m21log("f23", f23);
        f = xjmax(f12, f13);
        f = xjmax(f, f23);
        if (f < MATH21_EPS) {
            m21error("no focus");
            return 0;
        }
        if (x1(3) != 0) {
            x1(3) = f;
        }
        if (x2(3) != 0) {
            x2(3) = f;
        }
        if (x3(3) != 0) {
            x3(3) = f;
        }

        math21_operator_linear_to(1 / math21_operator_norm(x1, 2), x1);
        math21_operator_linear_to(1 / math21_operator_norm(x2, 2), x2);
        math21_operator_linear_to(1 / math21_operator_norm(x3, 2), x3);

        // compute T
        MatR K_inv(3, 3), R(3, 3);

        K_inv =
                1 / f, 0, -x0(1) / f,
                0, 1 / f, -x0(2) / f,
                0, 0, 1;
        R =
                x1(1), x2(1), x3(1),
                x1(2), x2(2), x3(2),
                x1(3), x2(3), x3(3);

        math21_operator_inverse(R);
        math21_operator_multiply(1.0, R, K_inv, T);
        return 1;
    }

    // compute s, T = S*T. Here we assume s = sx = sy.
    NumN geometry_cal_projection_scale(const VecR &xa, const VecR &xb, NumR l, MatR &T) {
        MATH21_ASSERT(xa.size() == 3 && xb.size() == 3)

        VecR Xa, Xb, Xab;
        NumN flag;
        flag = geometry_project(T, xa, Xa);
        if (flag == 0) {
            m21error("project xa");
            return 0;
        }
        flag = geometry_project(T, xb, Xb);
        if (flag == 0) {
            m21error("project xb");
            return 0;
        }

        math21_operator_linear(1.0, Xa, -1.0, Xb, Xab);
        if (math21_operator_norm(Xab, 2) < MATH21_EPS) {
            m21error("Xab.norm(2) too small");
            return 0;
        }
        NumR s = l / math21_operator_norm(Xab, 2);
        if (s < MATH21_EPS) {
            m21error("s too small");
            return 0;
        }
//        m21log("s", s);
//        T.log("T");
        MatR S;
        S.setSize(3, 3);
        S =
                s, 0, 0,
                0, s, 0,
                0, 0, 1;
        MatR T_tmp;
        math21_operator_multiply(1.0, S, T, T_tmp);
        T.assign(T_tmp);
        return 1;
    }

    // compute sx, sy, T = S*T. Here just get A, b.
    NumB geometry_cal_projection_scale_subroutine_01(const Seqce<VecR> &xas, const Seqce<VecR> &xbs,
                                                     const Seqce<NumR> &ls, const MatR &T,
                                                     MatR &A, VecR &b, VecR &ls_predict) {
        NumN N = xas.size();
        MATH21_ASSERT(N == 2 && xbs.size() == N && ls.size() == N);
        for (NumN i = 1; i <= N; ++i) {
            MATH21_ASSERT(xas(i).size() == 3 && xbs(i).size() == 3)
        }

        if (A.nrows() != N || A.ncols() != 2) {
            A.setSize(N, 2);
        }
        if (b.size() != N) {
            b.setSize(N);
        }
        if (ls_predict.size() != N) {
            ls_predict.setSize(N);
        }
        VecR Xab;
        NumN flag;

        for (NumN i = 1; i <= N; ++i) {
            const VecR &xa = xas(i);
            const VecR &xb = xbs(i);
            const NumR &l = ls(i);

            flag = geometry_project(T, xa, xb, Xab);
            if (flag == 0) {
                m21error("project xa, xb to Xab");
                return 0;
            }
            if (math21_operator_norm(Xab, 2) < MATH21_EPS) {
                m21error("Xab.norm(2) too small");
                return 0;
            }
            A(i, 1) = xjsquare(Xab(1));
            A(i, 2) = xjsquare(Xab(2));
            b(i) = xjsquare(l);
            ls_predict(i) = math21_operator_norm(Xab, 2);
        }
//        ls_predict.log("ls_predict");
        return 1;
    }

    // compute sx, sy, T = S*T.
    NumB geometry_cal_projection_scale_subroutine_02(MatR &A, VecR &b, MatR &T) {
        MatR S;
        S.setSize(3, 3);

//        A.log("A");
        VecR x;
        math21_operator_solve_linear_equation(A, b, x);

        if (x(1) < MATH21_EPS || x(2) < MATH21_EPS) {
            m21error("solution x of linear equation not positive");
            A.log("A");
            b.log("b");
            x.log("x");
            return 0;
        }
        NumR sx = xjsqrt(x(1));
        NumR sy = xjsqrt(x(2));
//        m21log("sx", sx);
//        m21log("sy", sy);
//        T.log("T");
        S =
                sx, 0, 0,
                0, sy, 0,
                0, 0, 1;
        MatR T_tmp;
        math21_operator_multiply(1.0, S, T, T_tmp);
        T.assign(T_tmp);
        return 1;
    }

    // compute sx, sy, T = S*T.
    NumB geometry_cal_projection_scale(const Seqce<VecR> &xas, const Seqce<VecR> &xbs,
                                       const Seqce<NumR> &ls, MatR &T) {
        NumN N = xas.size();
        MATH21_ASSERT(N == 2 && xbs.size() == N && ls.size() == N);
        for (NumN i = 1; i <= N; ++i) {
            MATH21_ASSERT(xas(i).size() == 3 && xbs(i).size() == 3)
        }

        MatR A;
        VecR b;
        VecR ls_predict;

        NumN flag;

        flag = geometry_cal_projection_scale_subroutine_01(xas, xbs,
                                                           ls, T,
                                                           A, b, ls_predict);
        if (flag == 0) {
            return 0;
        }

        flag = geometry_cal_projection_scale_subroutine_02(A, b, T);
        if (flag == 0) {
            return 0;
        }

        flag = geometry_cal_projection_scale_subroutine_01(xas, xbs,
                                                           ls, T,
                                                           A, b, ls_predict);
        if (flag == 0) {
            return 0;
        }
        return 1;
    }

    // X = T*x, xs * T_trans = Xs
    NumB geometry_cal_affine(const MatR &xs, const MatR &Xs, MatR &T) {
        MATH21_ASSERT(xs.isSameSize(3, 3));
        MATH21_ASSERT(Xs.isSameSize(3, 3));
        math21_operator_solve_linear_equation(xs, Xs, T);
        math21_operator_matrix_trans(T);
//        T.log("affine T");
        MATH21_ASSERT(math21_operator_num_isEqual(T(3, 1), 0, 1e-10));
        MATH21_ASSERT(math21_operator_num_isEqual(T(3, 2), 0, 1e-10));
        MATH21_ASSERT(math21_operator_num_isEqual(T(3, 3), 1, 1e-10));
        return 1;
    }

    // diag = {(x1,y1), (x2, y2)} as diagonal.
    // X = T * x, x in rectangle with diagonal diag, X in image(nr, nc).
    NumB geometry_cal_affine_matrix_axis_to_matrix(MatR &T, NumR x1, NumR x2, NumR y1, NumR y2, NumN nr, NumN nc) {
        NumR y_min, y_max, x_min, x_max;
        x_min = xjmin(x1, x2);
        y_min = xjmin(y1, y2);
        x_max = xjmax(x1, x2);
        y_max = xjmax(y1, y2);
        MatR xs(3, 3);
        MatR Xs(3, 3);
        xs =
                x_min, y_min, 1,
                x_max, y_min, 1,
                x_min, y_max, 1;
        Xs =
                nr, 1, 1,
                nr, nc, 1,
                1, 1, 1;
        return geometry_cal_affine(xs, Xs, T);
    }

    // compute affine matrix W, T = W*T.
    NumB geometry_cal_projection_to_world(const MatR &Xs, const MatR &xs, MatR &T) {
        MATH21_ASSERT(Xs.isSameSize(3, 3));
        MATH21_ASSERT(xs.isSameSize(3, 3));
        for (NumN i = 1; i <= 3; ++i) {
            MATH21_ASSERT(Xs(i, 3) == 1 && xs(i, 3) == 1)
        }

        MatR Xs_tilde;
        MatR W;
        NumB flag;
        flag = geometry_project_in_batch(T, xs, Xs_tilde);
        if (flag == 0) {
            m21error("project xs");
            return 0;
        }

        // Xs_tilde * W_trans = Xs.
        geometry_cal_affine(Xs_tilde, Xs, W);
        math21_operator_multiply_to_B(1.0, W, T);
        return 1;
    }

    // X = lambda * T * x; lambda is set to make sure X(3) = 1. Here T is 3*3.
    NumN geometry_project(const MatR &T, const VecR &x, VecR &X) {
        math21_operator_multiply(1.0, T, x, X);
        if (xjabs(X(3)) < MATH21_EPS) {
            return 0;
        }
        math21_operator_linear_to(1.0 / X(3), X);
        return 1;
    }

    // X = lambda * T * x; lambda is set to make sure X(3) = 1. Here T is 3*3.
    NumB geometry_project_in_batch(const MatR &T, const MatR &xs, MatR &Xs) {
        math21_operator_multiply_trans(1, xs, T, Xs);

        NumR ratio;
        for (NumN i = 1; i <= Xs.nrows(); ++i) {
            if (xjabs(Xs(i, 3)) < MATH21_EPS) {
                return 0;
            }
            ratio = Xs(i, 3);
            Xs(i, 1) /= ratio;
            Xs(i, 2) /= ratio;
            Xs(i, 3) = 1;
        }
        return 1;
    }

    // Xab = Xb - Xa. Here T is 3*3.
    NumN geometry_project(const MatR &T, const VecR &xa, const VecR &xb, VecR &Xab) {

        NumN flag;
        VecR Xa, Xb;
        flag = geometry_project(T, xa, Xa);
        if (flag == 0) {
            m21error("project xa");
            return 0;
        }
        flag = geometry_project(T, xb, Xb);
        if (flag == 0) {
            m21error("project xb");
            return 0;
        }
        math21_operator_linear(1.0, Xa, -1.0, Xb, Xab);
        return 1;
    }

    // index(1), index(2) keeps the X(1), X(2) respectively.
    // X = lambda * T * x; x is in image(nr, nc);
    NumB geometry_project_image(const MatR &T, NumN nr, NumN nc, TenR &index) {
        if (!index.isSameSize(2, nr, nc)) {
            index.setSize(2, nr, nc);
        }

        VecR x(3), X(3);
        for (NumN i = 1; i <= nr; ++i) {
            for (NumN j = 1; j <= nc; ++j) {
                x = i, j, 1;
                geometry_project(T, x, X);
                index(1, i, j) = X(1);
                index(2, i, j) = X(2);
            }
        }
        return 1;
    }

    // project points in [x1, x2]*[y1, y2] to [1, nr]*[1, nc].
    // src_index(1), src_index(2) keeps the original x, y position respectively.
    // X = lambda * T * x;
    NumB geometry_project_image(const MatR &T, NumR x1, NumR x2, NumR y1, NumR y2, NumN nr, NumN nc, TenR &src_index) {
        NumN i;
        VecR xa(3), xb(3), xc(3), xd(3);
        xa = x1, y1, 1;
        xb = x2, y1, 1;
        xc = x2, y2, 1;
        xd = x1, y2, 1;

        VecR Xa(3), Xb(3), Xc(3), Xd(3);
        geometry_project(T, xa, Xa);
        geometry_project(T, xb, Xb);
        geometry_project(T, xc, Xc);
        geometry_project(T, xd, Xd);

        NumR scale, scale_y, scale_x;
        NumR y_min, y_max, x_min, x_max;
        x_min = Xa(1);
        y_min = Xa(2);
        x_max = x_min;
        y_max = y_min;
        i = 1;
        while (i <= 4) {
            VecR *X_p = 0;
            switch (i) {
                case 1:
                    X_p = &Xa;
                    break;
                case 2:
                    X_p = &Xb;
                    break;
                case 3:
                    X_p = &Xc;
                    break;
                case 4:
                    X_p = &Xd;
                    break;
            }
            VecR &X = *X_p;
            if (x_min > X(1)) {
                x_min = X(1);
            }
            if (y_min > X(2)) {
                y_min = X(2);
            }
            if (x_max < X(1)) {
                x_max = X(1);
            }
            if (y_max < X(2)) {
                y_max = X(2);
            }
            ++i;
        }
//        m21log("warp img border", x_min, y_min, x_max, y_max);
        if (x_min >= x_max || y_min >= y_max) {
            m21error("warp img border error");
            return 0;
        }
        scale_x = nr / (x_max - x_min);
        scale_y = nc / (y_max - y_min);
        scale = xjmin(scale_y, scale_x);

        VecR t(3);
        t = x_min, y_min, 1;
        math21_operator_linear_to(scale, t);

        if (!src_index.isSameSize(2, nr, nc)) {
            src_index.setSize(2, nr, nc);
        }

        MatR T_inv;
        math21_operator_inverse(T, T_inv);
        VecR X(3), x(3);
        for (NumN i = 1; i <= nr; ++i) {
            for (NumN j = 1; j <= nc; ++j) {
                X = i, j, 0;
                math21_operator_linear_to_B(1.0, t, 1.0, X);
                math21_operator_linear_to(1 / scale, X);
                geometry_project(T_inv, X, x);
                src_index(1, i, j) = x(1);
                src_index(2, i, j) = x(2);
            }
        }
        return 1;
    }

    //scale x in [a,b] to [c, d]
    void scale(const MatR &m, MatR &C, NumR a, NumR b, NumR c, NumR d) {
        NumN i, j;
        NumN nr = m.nrows();
        NumN nc = m.ncols();
        MATH21_ASSERT(b > a && d > c, "scale range negative");
        MATH21_ASSERT(nr >= 1 && nc >= 1, "empty matrix");
        if (!C.isSameSize(m.shape())) {
            C.setSize(nr, nc);
        }
        NumR s = (d - c) / (b - a);
        for (i = 1; i <= nr; i++) {
            for (j = 1; j <= nc; j++) {
                C(i, j) = (m(i, j) - a) * s + c;
            }
        }
    }

    //scale x in [a,b] to [c, d], Y can't be X. if element of matrix X is all same, and c=0, d=1, then X will be 0.5.
    void scale(const TenR &X, TenR &Y, NumR c, NumR d) {
        MATH21_ASSERT(X.dims() == 2 && X.dim(2) == 1, "not satisfied in current version!")
        MATH21_ASSERT(d > c, "scale range negative");
        NumN i1, i2;
        NumR a, b;
        a = X(1, 1);
        b = X(1, 1);
        NumR tmp;
        for (i1 = 1; i1 <= X.dim(1); i1++) {
            for (i2 = 1; i2 <= X.dim(2); i2++) {
                tmp = X(i1, i2);
                if (tmp < a)a = tmp;
                if (tmp > b)b = tmp;
            }
        }
        if (!Y.isSameSize(X.shape())) {
            Y.setSize(X.shape());
        }
        if (b == a) {
            tmp = (d + c) / 2;
            for (i1 = 1; i1 <= X.dim(1); i1++) {
                for (i2 = 1; i2 <= X.dim(2); i2++) {
                    Y(i1, i2) = tmp;
                }
            }
        } else {
            NumR s = (d - c) / (b - a);
            for (i1 = 1; i1 <= X.dim(1); i1++) {
                for (i2 = 1; i2 <= X.dim(2); i2++) {
                    Y(i1, i2) = (X(i1, i2) - a) * s + c;
                }
            }
        }
    }

    void sample_sine_voice(TenR &X, TenR &Y, NumN seconds) {
        const NumR pi2 = XJ_PI * 2;
        const NumN sample_freq = 44100;
//        const NumN sample_freq = 10;
        const NumN n_samples = seconds * sample_freq;
//        uint16_t  ampl;
//        uint8_t bytes[2];
        X.setSize(n_samples, 1);
        Y.setSize(n_samples, 1);
        for (NumN i = 1, t = 0; i <= n_samples; i++, t++) {
//            ampl =(uint16_t)( NumN16_MAX * 0.5 * (1.0 + sin(pi2 * t * 1000.0 / sample_freq)));
//            bytes[0] = ampl>>8;
//            bytes[1] = ampl&0xff;
            X(i, 1) = t;
//            Y(i, 1) = NumN16_MAX * 0.5 * (1.0 + sin(pi2 * t * 1000.0 / sample_freq));
            Y(i, 1) = NumN16_MAX * 0.5 * (1.0 + sin(pi2 * t / sample_freq));
        }
    }

    void math21_sample_heart(MatR &A) {
        NumN n = 5000;
        A.setSize(n, 2);
        NumR t;
        for (NumN i = 1; i <= n; ++i) {
            t = 2.0 * MATH21_PI * i / n;
            A(i, 1) = 16 * m21_pow(xjsin(t), 3);
            A(i, 2) = 13 * xjcos(t) - 5 * xjcos(2 * t) - 2 * xjcos(3 * t) - xjcos(4 * t);
        }
    }


    //draw data to matrix with right hand axis.
    //matrix_axis is a matrix with elements 0 or 1, axis_x, axis_y is size of the matrix.
    void
    draw_data_at_matrix_axis(const TenR &X_sample, const TenR &Y_sample, TenN &matrix_axis, NumN axis_x, NumN axis_y) {
        if (X_sample.isEmpty()) {
            return;
        }
        MATH21_ASSERT(X_sample.dims() == 2 && X_sample.dim(2) == 1 &&
                      math21_operator_container_isEqual(X_sample.shape(), Y_sample.shape()),
                      "not satisfied in current version!")
        TenR X, Y;
        matrix_axis.setSize(axis_x, axis_y);
        scale(X_sample, X, 1, axis_x);
        scale(Y_sample, Y, 1, axis_y);
        NumN j1, j2;
        for (NumN i1 = 1; i1 <= X.dim(1); i1++) {
            for (NumN i2 = 1; i2 <= X.dim(2); i2++) {
                j1 = (NumN) X(i1, i2);
                j2 = (NumN) Y(i1, i2);
                if (matrix_axis(j1, j2) == 0) {
                    matrix_axis(j1, j2) = 1;
                }
            }
        }
    }

    /////////////////////////

    //! returns 3x3 affine transformation matrix for the planar scale and translation.
    // [a1, b1]*[a2,b2] -> [c1, d1]*[c2, d2]
    void la_getScaleTranslationMatrix2D(MatR &M,
                                        NumR a1, NumR b1, NumR a2, NumR b2,
                                        NumR c1, NumR d1, NumR c2, NumR d2) {
        // need to prove
        if (c1 == d1) {
            d1 = c1 + 1;
        }
        if (c2 == d2) {
            d2 = c2 + 1;
        }

        MATH21_ASSERT(a1 <= b1 && a2 <= b2 && c1 < d1 && c2 < d2);
        NumR k1, t1, k2, t2;
        if (a1 == b1) {
            k1 = 1;
        } else {
            k1 = (d1 - c1) / (b1 - a1);
        }
        if (a2 == b2) {
            k2 = 1;
        } else {
            k2 = (d2 - c2) / (b2 - a2);
        }
        t1 = c1 - k1 * a1;
        t2 = c2 - k2 * a2;
        if (!M.isSameSize(3, 3)) {
            M.setSize(3, 3);
        }
        M =
                k1, 0, t1,
                0, k2, t2,
                0, 0, 1;
    }

    void math21_la_getScaleTranslationMatrix2D(MatR &M,
                                               const Interval2D &input,
                                               Interval2D &output) {
        la_getScaleTranslationMatrix2D(M,
                                       input(1).left(), input(1).right(), input(2).left(), input(2).right(),
                                       output(1).left(), output(1).right(), output(2).left(), output(2).right());
    }

    void math21_la_2d_matrix_compute_matrix_axis_to_matrix(NumN axis_x, NumN axis_y, MatR &T_final) {
        MatR T;
        if (!T_final.isSameSize(3, 3)) {
            T_final.setSize(3, 3);
        }
        math21_operator_mat_eye(T_final);

        math21_la_2d_matrix_rotate(MATH21_PI_2, T);
        math21_operator_multiply_to_B(1, T, T_final);
        math21_la_2d_matrix_translate(axis_y, 0, T);
        math21_operator_multiply_to_B(1, T, T_final);
    }

    void math21_la_2d_matrix_compute_matrix_to_matrix_axis(NumN axis_x, NumN axis_y, MatR &T_final) {
        MatR T;
        if (!T_final.isSameSize(3, 3)) {
            T_final.setSize(3, 3);
        }
        math21_operator_mat_eye(T_final);

        math21_la_2d_matrix_translate(-(NumZ) axis_y, 0, T);
        math21_operator_multiply_to_B(1, T, T_final);
        math21_la_2d_matrix_rotate(-MATH21_PI_2, T);
        math21_operator_multiply_to_B(1, T, T_final);
    }

    void la_data_2d_bound(const MatR &A,
                          NumR &a1, NumR &b1, NumR &a2, NumR &b2) {
        MATH21_ASSERT(A.dims() == 2 && A.dim(2) == 2)
        NumN i;
        NumN nr = A.dim(1);
        a1 = A(1, 1);
        a2 = A(1, 2);
        b1 = a1;
        b2 = a2;
        NumR tmp;
        for (i = 1; i <= nr; ++i) {
            tmp = A(i, 1);
            if (tmp < a1) {
                a1 = tmp;
            } else if (tmp > b1) {
                b1 = tmp;
            }
            tmp = A(i, 2);
            if (tmp < a2) {
                a2 = tmp;
            } else if (tmp > b2) {
                b2 = tmp;
            }
        }
    }

    // A: nr * (x, y) or y with x setting to index.
    void math21_la_data_2d_bound(const TenR &A,
                                 NumR &a1, NumR &b1, NumR &a2, NumR &b2) {
        MATH21_ASSERT((A.dims() == 2 && A.dim(2) == 2)
                      || (A.dims() == 1))

        if (A.dims() == 2) {
            TenR B;
            VecN index(A.dims());
            index = 1, 0;

            TensorFunction_min f_min;
            math21_operator_tensor_f_shrink(A, B, index, f_min);
            a1 = B(1);
            a2 = B(2);

            TensorFunction_max f_max;
            math21_operator_tensor_f_shrink(A, B, index, f_max);
            b1 = B(1);
            b2 = B(2);
        } else {
            TenR B;
            VecN index(A.dims());
            index = 1;

            TensorFunction_min f_min;
            math21_operator_tensor_f_shrink(A, B, index, f_min);
            a1 = 1;
            a2 = B(1);

            TensorFunction_max f_max;
            math21_operator_tensor_f_shrink(A, B, index, f_max);
            b1 = A.dim(1);
            b2 = B(1);
        }

    }

    // A: nr * (x, y) or y with x setting to index.
    void math21_la_data_2d_bound(const TenR &A,
                                 Interval2D &I) {
        NumR a1, b1, a2, b2;
        math21_la_data_2d_bound(A, a1, b1, a2, b2);
        I(1).set(a1, b1, 1, 1);
        I(2).set(a2, b2, 1, 1);
    }

    // A: batch * nr * (x, y) or batch * y with x setting to index.
    void math21_la_data_2d_bound_in_batch(const TenR &A,
                                          NumR &a1, NumR &b1, NumR &a2, NumR &b2) {
        MATH21_ASSERT((A.dims() == 3 && A.dim(3) == 2)
                      || (A.dims() == 2))

        if (A.dims() == 3) {
            TenR B;
            VecN index(A.dims());
            index = 1, 1, 0;

            TensorFunction_min f_min;
            math21_operator_tensor_f_shrink(A, B, index, f_min);
            a1 = B(1);
            a2 = B(2);

            TensorFunction_max f_max;
            math21_operator_tensor_f_shrink(A, B, index, f_max);
            b1 = B(1);
            b2 = B(2);
        } else {
            TenR B;
            VecN index(A.dims());
            index = 1, 1;

            TensorFunction_min f_min;
            math21_operator_tensor_f_shrink(A, B, index, f_min);
            a1 = 1;
            a2 = B(1);

            TensorFunction_max f_max;
            math21_operator_tensor_f_shrink(A, B, index, f_max);
            b1 = A.dim(2);
            b2 = B(1);
        }

    }

    // A: batch * nr * (x, y) or batch * y with x setting to index.
    void math21_la_data_2d_bound_in_batch(const TenR &A,
                                          Interval2D &I) {
        NumR a1, b1, a2, b2;
        math21_la_data_2d_bound_in_batch(A, a1, b1, a2, b2);
        I(1).set(a1, b1, 1, 1);
        I(2).set(a2, b2, 1, 1);
    }

    void la_scale_data_2d(const MatR &data, MatR &data_new,
                          NumR c1, NumR d1, NumR c2, NumR d2) {
        if (data.isEmpty()) {
            return;
        }
        MATH21_ASSERT(data.dims() == 2 && data.dim(2) == 2)
        MATH21_ASSERT(c1 < d1 && c2 < d2);
        NumR a1, b1, a2, b2;
        la_data_2d_bound(data, a1, b1, a2, b2);
        MatR M;
        la_getScaleTranslationMatrix2D(M, a1, b1, a2, b2,
                                       c1, d1, c2, d2);
        if (!data_new.isSameSize(data.shape())) {
            data_new.setSize(data.shape());
        }
        NumN nr = data.dim(1);
        NumN i;
        VecR x(3), y(3);
        x(3) = 1;
        for (i = 1; i <= nr; ++i) {
            x(1) = data(i, 1);
            x(2) = data(i, 2);
            math21_operator_multiply(1, M, x, y);
            data_new(i, 1) = y(1);
            data_new(i, 2) = y(2);
        }
    }

    // data: (x, y) or y with x setting to index.
    void la_scale_data_2d(const MatR &data, MatR &data_new,
                          const MatR &M) {
        if (data.isEmpty()) {
            return;
        }
        MATH21_ASSERT((data.dims() == 2 && data.dim(2) == 2) || data.dims() == 1)
        MATH21_ASSERT(M.isSameSize(3, 3));
        NumN nr = data.dim(1);
        if (!data_new.isSameSize(nr, 2)) {
            data_new.setSize(nr, 2);
        }
        NumN i;
        VecR x(3), y(3);
        x(3) = 1;
        if (data.dims() == 2) {
            for (i = 1; i <= nr; ++i) {
                x(1) = data(i, 1);
                x(2) = data(i, 2);
                math21_operator_multiply(1, M, x, y);
                data_new(i, 1) = y(1);
                data_new(i, 2) = y(2);
            }
        } else {
            for (i = 1; i <= nr; ++i) {
                x(1) = i;
                x(2) = data(i);
                math21_operator_multiply(1, M, x, y);
                data_new(i, 1) = y(1);
                data_new(i, 2) = y(2);
            }
        }
    }

    void la_scale_data_2d(MatR &data,
                          const MatR &M) {
        MatR data_new;
        la_scale_data_2d(data, data_new, M);
        data.swap(data_new);
    }

    void la_draw_data_2d_at_matrix_axis_no_scale(const MatR &data, TenN &matrix_axis) {
        if (data.isEmpty()) {
            return;
        }
        MATH21_ASSERT(data.dims() == 2 && data.dim(2) == 2,
                      "not satisfied in current version!")
        MATH21_ASSERT(matrix_axis.dims() == 2 && matrix_axis.dim(1) > 0 && matrix_axis.dim(2) > 0)
        NumN axis_x = matrix_axis.dim(1);
        NumN axis_y = matrix_axis.dim(2);

        NumN j1, j2;
        NumN nr;
        nr = data.dim(1);

//#pragma omp parallel for private(j1, j2)
        for (NumN i1 = 1; i1 <= nr; i1++) {
            j1 = (NumN) data(i1, 1);
            j2 = (NumN) data(i1, 2);
            if (xjIsIn(j1, 1, axis_x) && xjIsIn(j2, 1, axis_y)) {
                if (matrix_axis(j1, j2) == 0) {
                    matrix_axis(j1, j2) = 1;
                }
            }
        }
    }


}