#pragma once

#include "platform_config.h"
#include "simcc_export.h"

#ifndef H_CASE_STRING_BIGIN
#define H_CASE_STRING_BIGIN(state) switch(state) {
#define H_CASE_STRING(state) case state: return #state; break;
#define H_CASE_STRING_END()  default: return "Unknown"; break; }
#endif

#include <string>

namespace simcc {

using std::string;
using std::wstring;

typedef uint64_t uint64;
typedef int64_t  int64;
typedef uint32_t uint32;
typedef int32_t  int32;
typedef uint16_t uint16;
typedef int16_t  int16;
typedef uint8_t  uint8;
typedef int8_t   int8;
typedef float    float32;
typedef double   float64;

}
