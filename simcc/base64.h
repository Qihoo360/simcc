#pragma once

#include "simcc/inner_pre.h"
#include <string.h>
#include <string>

namespace simcc {

class SIMCC_EXPORT Base64 {
public:
    // @brief b64 encode a c-style string with len
    // 
    // @param[in] s the input string to encode
    // @param[in] len the length of the input string
    // @return a newly allocated b64 string.  Empty if failed.
    static string Encode(const char* s, size_t len);

    // @brief b64 encode a c-style string
    // 
    // @param[in] s the input string to encode
    // @return a newly allocated b64 string.  Empty if failed.
    static string Encode(const char* s) {
        return Encode(s, static_cast<int>(strlen(s)));
    }

    // @brief b64 encode a const string
    // 
    // @param[in] s the input string to encode
    // @return a newly allocated b64 string.  Empty if failed.
    static string Encode(const string& s) {
        return Encode(s.data(), s.size());
    }

    static string Decode(const char* src, size_t len);

    static string Decode(const char* src) {
        return Decode(src, strlen(src));
    }

    static string Decode(const string& s) {
        return Decode(s.data(), s.size());
    }
};

}
