#pragma once

#ifdef _DEBUG
#ifndef H_DEBUG_MODE
#define H_DEBUG_MODE
#endif
#endif

#include <assert.h>
#include <stdint.h>

#ifdef __cplusplus
#include <iostream>
#include "gettimeofday.h"
#include <memory>
#include <functional>
#endif // end of define __cplusplus

// from google3/base/basictypes.h
// The H_ARRAYSIZE(arr) macro returns the # of elements in an array arr.
// The expression is a compile-time constant, and therefore can be
// used in defining new arrays, for example.
//
// H_ARRAYSIZE catches a few type errors.  If you see a compiler error
//
//   "warning: division by zero in ..."
//
// when using H_ARRAYSIZE, you are (wrongfully) giving it a pointer.
// You should only use H_ARRAYSIZE on statically allocated arrays.
//
// The following comments are on the implementation details, and can
// be ignored by the users.
//
// ARRAYSIZE(arr) works by inspecting sizeof(arr) (the # of bytes in
// the array) and sizeof(*(arr)) (the # of bytes in one array
// element).  If the former is divisible by the latter, perhaps arr is
// indeed an array, in which case the division result is the # of
// elements in the array.  Otherwise, arr cannot possibly be an array,
// and we generate a compiler error to prevent the code from
// compiling.
//
// Since the size of bool is implementation-defined, we need to cast
// !(sizeof(a) & sizeof(*(a))) to size_t in order to ensure the final
// result has type size_t.
//
// This macro is not perfect as it wrongfully accepts certain
// pointers, namely where the pointer size is divisible by the pointer
// size.  Since all our code has to go through a 32-bit compiler,
// where a pointer is 4 bytes, this means all pointers to a type whose
// size is 3 or greater than 4 will be (righteously) rejected.
//
// Kudos to Jorg Brown for this simple and elegant implementation.
#undef H_ARRAYSIZE
#define H_ARRAYSIZE(a) \
    ((sizeof(a) / sizeof(*(a))) / \
     static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))


/*
* Sizes for buffers used by the _makepath() and _splitpath() functions.
* note that the sizes include space for 0-terminator
*/
#ifndef _MAX_PATH
#   define _MAX_PATH   260 /* max. length of full pathname */
#endif

#include "windows_port.h"