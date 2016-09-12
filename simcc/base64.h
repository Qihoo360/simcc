#pragma once

#include "simcc/simcc_export.h"
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
    static std::string Encode(const char* s, size_t len);

    // @brief b64 encode a c-style string
    // 
    // @param[in] s the input string to encode
    // @return a newly allocated b64 string.  Empty if failed.
    static std::string Encode(const char* s) {
        return Encode(s, static_cast<int>(strlen(s)));
    }

    // @brief b64 encode a const std::string
    // 
    // @param[in] s the input string to encode
    // @return a newly allocated b64 string.  Empty if failed.
    static std::string Encode(const std::string& s) {
        return Encode(s.data(), s.size());
    }

    // base 64 encode a string (self-modifying)
    // 
    // This function is for C++ only (duh)
    // 
    // @param[in,out] s the string to be decoded
    // @return a reference to the input string
    static std::string& Encode(std::string& s) {
        std::string x(Encode(s.data(), s.size()));
        s.swap(x);
        return s;
    }

    static std::string Decode(const char* src, size_t len);

    static std::string Decode(const char* src) {
        return Decode(src, strlen(src));
    }

    // base 64 decode a string (self-modifying)
    // On failure, the string is empty.
    // 
    // This function is for C++ only
    // 
    // @param[in,out] s the string to be decoded
    // @return a reference to the input string
    static std::string& Decode(std::string& s) {
        std::string x(Decode(s.data(), s.size()));
        s.swap(x);
        return s;
    }

    static std::string Decode(const std::string& s) {
        return Decode(s.data(), s.size());
    }
};

}
