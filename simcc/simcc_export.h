#pragma once

// Define Operation System.
#if ( defined(WIN32) || defined(WIN64) )
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
