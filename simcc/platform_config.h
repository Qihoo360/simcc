#pragma once

#include "simcc_export.h"

#ifdef _DEBUG
#ifndef H_DEBUG_MODE
#define H_DEBUG_MODE
#endif
#endif


#include <assert.h>
#include <stdint.h>

#include <iostream>

#ifdef __cplusplus

#include "gettimeofday.h"

#include <memory>
#include <functional>

// Finds the compiler type and version.
#if defined( _MSC_VER )
#   ifndef H_COMPILER_MSVC
#       define H_COMPILER_MSVC
#   endif
#   ifndef H_COMPILER_VERSION
#       define H_COMPILER_VERSION _MSC_VER
#   endif
#   if H_COMPILER_VERSION < 1300
#       pragma error "Not supported compiler version. Abort! Abort!"
#   endif
#elif defined( __GNUC__ )
#   ifndef H_COMPILER_GNUC
#       define H_COMPILER_GNUC
#   endif
#   ifndef H_COMPILER_VERSION
#       define H_COMPILER_VERSION (((__GNUC__)*100) + \
                                   (__GNUC_MINOR__ * 10) + \
                                   __GNUC_PATCHLEVEL__)
#   endif
#else
#   pragma error "No known compiler. Abort! Abort!"
#endif

#endif // end of define __cplusplus

#ifdef H_OS_WINDOWS
#define usleep(us) Sleep((us)/1000);
#define snprintf  _snprintf
#define thread_local __declspec( thread )
#endif

#ifdef H_OS_WINDOWS
#pragma warning( disable: 4005 ) // warning C4005 : 'va_copy' : macro redefinition
#pragma warning( disable: 4251 )
#pragma warning( disable: 4996 ) // warning C4996: 'strerror': This function or variable may be unsafe. Consider using strerror_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
#pragma warning( disable: 4244 4251 4355 4715 4800 4996 4005 4819)
#endif

// get rid of Windows/Linux inconsistencies
#ifdef H_OS_WINDOWS
#   ifndef PRIu64
#       define PRIu64     "I64u"
#   endif
#else
#   ifndef PRIu64
#       define PRIu64     "lu"
#   endif
#endif

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

#ifdef H_OS_WINDOWS
#   define H_LITTLE_ENDIAN
#elif defined(H_OS_FREEBSD)
#   include <sys/endian.h>
#else
#   include <endian.h>
#   if __BYTE_ORDER == __LITTLE_ENDIAN
#       define H_LITTLE_ENDIAN
#   else
#       define H_BIG_ENDIAN
#   endif
#endif


#ifdef H_OS_WINDOWS
#   define strcasecmp stricmp
#   define strncasecmp strnicmp
#else
#   define stricmp strcasecmp //Linux
#   define strnicmp strncasecmp //Linux
#endif

/*
* Sizes for buffers used by the _makepath() and _splitpath() functions.
* note that the sizes include space for 0-terminator
*/
#ifndef _MAX_PATH
#   define _MAX_PATH   260 /* max. length of full pathname */
#endif
