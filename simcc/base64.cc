#include "base64.h"
#include "misc/modp_b64.h"

namespace simcc {

string Base64::Encode(const char* s, size_t len) {
    return modp::b64_encode(s, len);
}

string Base64::Decode(const char* src, size_t len) {
    return modp::b64_decode(src, len);
}

}