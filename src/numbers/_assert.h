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

#include "config.h"
#include <sstream>
#include "error.h"

namespace math21 {


// Use some stuff from dlib here
// License: Boost Software License   See LICENSE.txt for the full license.

// breakpoints
    extern "C"
    {
    inline void math21_assert_breakpoint(
    ) {}
/*!
    ensures
        - this function does nothing
          It exists just so you can put breakpoints on it in a debugging tool.
          It is called only when an MATH21_ASSERT or MATH21_CASSERT fails and is about to
          throw an exception.
!*/
    }

#if defined MATH21_DISABLE_ASSERTS
    // if MATH21_DISABLE_ASSERTS is on then never enable MATH21_ASSERT no matter what.
#undef MATH21_ENABLE_ASSERTS
#endif

#if !defined(MATH21_DISABLE_ASSERTS) && (defined MATH21_DEBUG)
// make sure MATH21_ENABLE_ASSERTS is defined if we are indeed using them.
#ifndef MATH21_ENABLE_ASSERTS
#define MATH21_ENABLE_ASSERTS
#endif
#endif


// -----------------------------

#ifdef __GNUC__
// There is a bug in version 4.4.5 of GCC on Ubuntu which causes GCC to segfault
// when __PRETTY_FUNCTION__ is used within certain templated functions.  So just
// don't use it with this version of GCC.
#  if !(__GNUC__ == 4 && __GNUC_MINOR__ == 4 && __GNUC_PATCHLEVEL__ == 5)
#    define MATH21_FUNCTION_NAME __PRETTY_FUNCTION__
#  else
#    define MATH21_FUNCTION_NAME "unknown function"
#  endif
#elif defined(_MSC_VER)
#define MATH21_FUNCTION_NAME __FUNCSIG__
#else
#define MATH21_FUNCTION_NAME "unknown function"
#endif


    template<typename ExpType, typename MsgType>
    inline void
    _math21_assert_print(const ExpType &expType, const MsgType &msgType, const char *file, const int line,
                         const char *func) {
        if (!(expType)) {
            math21_assert_breakpoint();
            std::ostringstream io;
            io << "\n\nEEError detected at line " << line << ".\n";
            io << "Error detected in file " << file << ".\n";
            io << "Error detected in function " << func << ".\n\n";
            io << "Failing expression was " << expType << ".\n";
            io << std::boolalpha << msgType << "\n";
            throw math21::fatal_error(math21::EBROKEN_ASSERT, io.str());
        }
    }

///////////////////////////////// CUDA ////////////////////////////////////
#ifdef MATH21_FLAG_USE_CUDA

    static inline void _math21_assert_cuda(cudaError_t err, const char *file, const int line,
                                           const char *func) {
        if (cudaSuccess != err) {
            _math21_assert_print(0, cudaGetErrorString(err), file, line, func);
        }
    }

#ifndef MATH21_ASSERT_CUDA_CALL
#define MATH21_ASSERT_CUDA_CALL(expr)  _math21_assert_cuda(expr, __FILE__, __LINE__, MATH21_FUNCTION_NAME)
#endif
#endif

/////////////////////////////////   ////////////////////////////////////

#define MATH21M_CASSERT(_exp, _message)                                              \
    {if ( !(_exp) )                                                         \
    {                                                                       \
        math21_assert_breakpoint();                                           \
        std::ostringstream io;                                       \
        io << "\n\nEEError detected at line " << __LINE__ << ".\n";    \
        io << "Error detected in file " << __FILE__ << ".\n";      \
        io << "Error detected in function " << MATH21_FUNCTION_NAME << ".\n\n";      \
        io << "Failing expression was " << #_exp << ".\n";           \
        io << std::boolalpha << _message << "\n";                    \
        throw math21::fatal_error(math21::EBROKEN_ASSERT,io.str());      \
    }}

//#define MATH21M_CASSERT(_exp, _message) _math21_assert_print(_exp, _message, __FILE__, __LINE__, MATH21_FUNCTION_NAME);

// Make it so the 2nd argument of MATH21_CASSERT is optional.  That is, you can call it like
// MATH21_CASSERT(exp) or MATH21_CASSERT(exp,message).
#define MATH21M_CASSERT_1_ARGS(exp)              MATH21M_CASSERT(exp,"")
#define MATH21M_CASSERT_2_ARGS(exp, message)      MATH21M_CASSERT(exp,message)
#define MATH21M_GET_3TH_ARG(arg1, arg2, arg3, ...) arg3
#define MATH21M_CASSERT_CHOOSER(...) MATH21M_GET_3TH_ARG(__VA_ARGS__,  MATH21M_CASSERT_2_ARGS, MATH21M_CASSERT_1_ARGS)
#define MATH21_CASSERT(...) MATH21M_CASSERT_CHOOSER(__VA_ARGS__)(__VA_ARGS__)


#ifdef MATH21_ENABLE_ASSERTS
#define MATH21_ASSERT_INDEX(...) {}
//#define MATH21_ASSERT_INDEX(...) MATH21_CASSERT(__VA_ARGS__)
#define MATH21_ASSERT(...) MATH21_CASSERT(__VA_ARGS__)
// this assert will slow running. So we can comment out to speed.
#define MATH21_ASSERT_SLOW(...) MATH21_CASSERT(__VA_ARGS__)
#define MATH21_PASS(...) MATH21_CASSERT(__VA_ARGS__)
#define MATH21_ASSERT_FINITE(...) MATH21_CASSERT(__VA_ARGS__)
#define MATH21_ASSERT_CHECK_VALUE_TMP(...) MATH21_CASSERT(__VA_ARGS__)
#define MATH21_ASSERT_CHECK_VALUE(...) MATH21_CASSERT(__VA_ARGS__)
// assert we write the good code. Once passed, we can comment this out.
#define MATH21_ASSERT_CODE(...) MATH21_CASSERT(__VA_ARGS__)
#define MATH21_IF_ASSERT(exp) exp
#else
#define MATH21_ASSERT_INDEX(...) {}
#define MATH21_ASSERT(...) {}
#define MATH21_PASS(...) {}
#define MATH21_ASSERT_FINITE(...) {}
#define MATH21_ASSERT_CODE(...) {}
#define MATH21_IF_ASSERT(exp)
#endif

#ifdef MATH21_ENABLE_ASSERTS_REMINDER
#define MATH21_ASSERT_REMINDER(...) MATH21_ASSERT(__VA_ARGS__)
#else
#define MATH21_ASSERT_REMINDER(...) {}
#endif

}