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

#include <typeinfo>
#include <iomanip>
/*
 * Document:
 * 1 NumN and NumZ. Use NumZ instead of NumN in some situations.
 * 2 vector, matrix, tensor are all tensor type. VecR and MatR all just for easy understand for user.
 * They are same as TenR. The only way to distinguish them is by their shape.
 * 3 Number is 0-d tensor, and is represented by tensor
 * with dims() = 1, and dim(1)=1, so it is a special vector.
 * 4 undefined reference to: template method must be defined in .h, not .cc files.
 * 5 If there is something strange happening, consider the possibility that index may be out of bound.
 * Open MATH21_ASSERT_INDEX to check.
 * 6 No checking in subroutine.
 * 7 there are some bugs when using Seqce<Seqce<T> >, but it's ok to use Seqce<T>.
 * Same to Seqce.
 * 8 matrix is nr, nc with origin sitting at top-left corner. Same to image.
 * matrix axis is (x, y) with origin sitting at bottom-left corner. They are all in right hand.
 * 9 init() functions can not call clear(). Because we check some values in clear function,
 * and these values may not have been initialized if we call clear() in init function.
 * 10 sort or sort by index
 * 11 vector, matrix, tensor deal with numbers.
 * so are set, sequence.
 * array are underlying structure, don't use directly.
 * seqce are like sequence, but can deal with any objects, like function, string, numbers, vector, tensor.
 * 12 Error:
 * lldb illegal instruction operand.
 * Cannot access memory at address, or something like this.
 * Object may not exist at this time. e.x., forget to return an object when function
 * requires that. So caller of this function will use an object which doesn't exist actually.
 * 13 In program, we have 3 major structures: number, container, tensor.
 * dim 1 tensor is container. But since tensor which dims is larger than 1 can have multiple vector representations like
 * row representation and col representation, we don't consider this kind of tensor as container, but we let tensor be container which
 * is one kind of vector representation.
 * In math, we currently have concepts such as set, sequence, function, map, number, vector, matrix, tensor.
 * 14 shareReshape: different tensors have some common data storage.
 * 15 Todo: remove tensor methods which is container methods essentially.
 * Tensor is container.
 * Extending size() to volume()
 * Extending at(i) to at(data(i))
 * If data is not continuously stored, you must subclass tensor to use.
 *
 * 16 Error: not converge in iterative method which uses random numbers.
 * Maybe random number generator is same in every iteration.
 * Solve: Put the random engine outside of the iterative process.
 * 17
 * Tensor is continuous, but subclass may not be.
 * But all of them can be container.
 * Data order in this container is not specified, i.e., it can be row-major or others.
 * But for vector as container, data order is same as vector order.
 * Now tensor view and tensor sub are column-major,
 * and this can be changed easily because data order is not specified by rule.
 * 18
 * tensor supports row-major and column-major data, but default is row-major.
 * Tensor is column-major saved only if you call setColumnMajor(1) and then call set data related methods.
 * If you only call setColumnMajor(1), data will not changed, but it is interpreted as column-major data,
 * even though it may be row-major previously.
 * 19 Safety
 * operators like math21_operator_container_linear (k, A, B) is safe only
 * a) A and B are same
 * or
 * b) data of A and B don't overlap.
 * 20 Android crash
 * Fatal signal 4 (SIGILL), code 1, fault addr
 * forget returning.
 * 21
 * extend tensor to function defined on Z.
 * 22 careful about comparing NumZ and NumN.
 * NumN a = 1;
 * NumZ b = -(NumZ)a;
 * So use NumZ when necessary.
 * 23
 * Don't become discouraged. Believe in yourself
 * 24 thread-safe
 * element access is thread-safe: Array,
 * Tensor, IndexFunctional.
 * 25 Todo: improve element access speed of IndexFunctional when parallel.
 * 26 All functions starting with math21_c only process continuous, row-major data.
 * 27 In Auto Buffer, function set size doesn't keep original data. New memory is not set to zero.
 *
 *
 * */
namespace math21 {
    typedef long NumZ;
    typedef unsigned long NumN;
    typedef double NumR; // default floating type
    typedef unsigned int NumB;

#define MATH21_TIME_MAX (10000000)
#define MATH21_OPT_TIME_MAX (10000000)
#define XJ_TRUE 1
#define XJ_FALSE 0

#define MATH21_EPS (0.000001)
#define MATH21_10NEG6 (0.000001)
#define MATH21_10NEG7 (0.0000001)
#define MATH21_EPS_NEG (-0.000001)
#define XJ_EPS (0.000001)
#define XJ_PI (3.14159265357989323)
#define XJ_MAX (10000000)
#define XJ_MIN (-10000000)

    typedef int Int; // 32 bit integer
    typedef unsigned int Uint;
    typedef int NumZ32; // 32 bit integer
    typedef unsigned int NumN32;
#ifdef _MSC_VER
    typedef __int64 Llong; // 64 bit integer
    typedef __int64 NumZ64; // 64 bit integer
    typedef unsigned __int64 Ullong;
    typedef unsigned __int64 NumN64;
#else
    typedef long long int Llong; // 64 bit integer
    typedef unsigned long long int Ullong;
    typedef long long int NumZ64; // 64 bit integer
    typedef unsigned long long int NumN64;
#endif

    typedef char Char; // 8 bit integer
    typedef unsigned char Uchar;
    typedef char NumZ8; // 8 bit integer
    typedef unsigned char NumN8;

    typedef double Doub; // default floating type
    typedef long double Ldoub;


#define NumZ8_MIN (-128)
#define NumZ16_MIN (-32768)
#define NumZ32_MIN (-2147483647 - 1)
#define NumZ64_MIN  (-9223372036854775807LL - 1)

#define NumZ8_MAX 127
#define NumZ16_MAX 32767
#define NumZ32_MAX 2147483647
#define NumZ64_MAX 9223372036854775807LL

#define NumN8_MAX 0xff /* 255U */
#define NumN16_MAX 0xffff /* 65535U */
#define NumN32_MAX 0xffffffff  /* 4294967295U */
#define NumN64_MAX 0xffffffffffffffffULL /* 18446744073709551615ULL */

    ///////////////

#define MATH21_MAX (NumZ32_MAX)
#define MATH21_MIN (NumZ32_MIN)

    ///////////////

    /* Some useful constants.  */
#define MATH21_E        2.7182818284590452354    /* e */
#define MATH21_LOG2E    1.4426950408889634074    /* log_2 e */
#define MATH21_LOG10E    0.43429448190325182765    /* log_10 e */
#define MATH21_LN2        0.69314718055994530942    /* log_e 2 */
#define MATH21_LN10        2.30258509299404568402    /* log_e 10 */
#define MATH21_PI        3.14159265358979323846    /* pi */
#define MATH21_PI_2        1.57079632679489661923    /* pi/2 */
#define MATH21_PI_4        0.78539816339744830962    /* pi/4 */
#define MATH21_1_PI        0.31830988618379067154    /* 1/pi */
#define MATH21_2_PI        0.63661977236758134308    /* 2/pi */
#define MATH21_2_SQRTPI    1.12837916709551257390    /* 2/sqrt(pi) */
#define MATH21_SQRT2    1.41421356237309504880    /* sqrt(2) */
#define MATH21_SQRT1_2    0.70710678118654752440    /* 1/sqrt(2) */

    ///////////////
//
    struct Interval;
    struct Interval2D;

    template<typename T>
    struct Seqce;
    typedef Seqce<NumN> SeqceN;
    typedef Seqce<NumZ> SeqceZ;
    typedef Seqce<NumR> SeqceR;
//    typedef SeqceN Sequence;

    template<typename T>
    struct _Set;

    typedef _Set<NumN> SetN;
    typedef _Set<NumZ> SetZ;
    typedef _Set<NumR> SetR;
    typedef SetN Set;

    // should deprecate, and use seqce instead.
    template<typename T>
    struct _Sequence;
    typedef _Sequence<NumN> SequenceN;
    typedef _Sequence<NumZ> SequenceZ;
    typedef _Sequence<NumR> SequenceR;
    typedef SequenceN Sequence;

    template<typename T, typename S>
    struct _Map;
    typedef _Map<NumN, NumN> MapNN;
    typedef _Map<NumZ, NumZ> MapZZ;
    typedef _Map<NumR, NumR> MapRR;
    typedef MapNN Map;

    template<typename T>
    class Tensor;
}