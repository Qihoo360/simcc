#pragma once

#include "simcc/simcc_export.h"
#include <stdint.h>
#include <string>

namespace simcc {

// CRC-16/CCITT
class SIMCC_EXPORT CRC16 {
public:
    static uint16_t Sum(const void* data, size_t len);
    static uint16_t Sum(const std::string& s) {
        return Sum(s.data(), s.size());
    }
};

}





