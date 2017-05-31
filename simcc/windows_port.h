#pragma once

// Define Operation System.
#if ( defined(_WIN32) || defined(WIN64) )
#   ifndef H_OS_WINDOWS
#       define H_OS_WINDOWS
#   endif
#   ifndef H_WINDOWS_API
#       define H_WINDOWS_API
#   endif
#endif
//
//// Module symbol export
//#ifdef H_WINDOWS_API
//#   ifndef  H_STATIC_LIB_SIMCC
//#       ifdef  SIMCC_EXPORTS
//#           define SIMCC_EXPORT __declspec(dllexport)
//#       else
//#           define SIMCC_EXPORT __declspec(dllimport)
//#       endif
//#   else
//#       define SIMCC_EXPORT
//#   endif
//#else
//#   define SIMCC_EXPORT
//#endif
//
#define SIMCC_EXPORT


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


#ifdef H_OS_WINDOWS
#ifndef SIMCC_EXPORTS
#pragma comment(lib, "simcc_static.lib")
#endif
#pragma comment(lib,"Ws2_32.lib")
#endif
