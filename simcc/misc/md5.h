#pragma once

#include "simcc/inner_pre.h"


#include "php_md5.h"

#include <fstream>
#include <iostream>

namespace simcc {

// A wrapper of PHP md5 implementation
class SIMCC_EXPORT MD5 {
public:
    enum {
        kBinDigestLength = 16, // the length of digest as a 16-byte binary array
        kHexDigestLength = 32, // the length of digest as a 32-byte ASCII-hex string, not include the terminating null-character
    };
public:
    MD5() {
        PHP_MD5Init(&md5_ctx);
    }

    void Update(const void* data, size_t data_len) {
        PHP_MD5Update(&md5_ctx, (const unsigned char*)data, data_len);
    }

    template<typename string_t>
    void Update(const string_t& data) {
        Update(data.data(), data.size());
    }

    void Finalize(unsigned char binary16_digest[/*16*/]) {
        PHP_MD5Final(binary16_digest, &md5_ctx);
    }

    void Finalizeh(char hex33[33]);

    std::string Finalize() {
        std::string m(kBinDigestLength, '\0');
        Finalize((unsigned char*)&m[0]);
        return m;
    }

    std::string Finalizeh() {
        std::string m(kHexDigestLength, '\0');
        Finalize((unsigned char*)&m[0]);
        //m.resize(kHexDigestLength);
        return m;
    }

    // static helpful utility methods
public:
    // Calculate the MD5 checksum of the data.
    static void Sum(const void* data, size_t data_len, unsigned char binary16_digest[16]);
    static std::string Sum(const void* data, size_t data_len);
    static std::string Sum(const std::string& d);

    // Calculate the MD5 checksum of the data. The checksum is human readable with 32 bytes hex code.
    static void Sumh(const void* data, size_t data_len, char hex33[/*33*/]);
    static std::string Sumh(const void* data, size_t data_len);
    static std::string Sumh(const std::string& d);

    // Generate a random md5
    // @return a random hex md5, 32 bytes
    static std::string GenHexMD5(const size_t random_data_len = 8);

    // @return a random binary md5, 16 bytes
    static std::string GenBinMD5(const size_t random_data_len = 8);

    // @brief Convert a binary buffer to a hex format
    // @param[in] binary - 
    // @param[in] binary_len - 
    // @param[in] hex - At least with length of binary_len*2
    // @return void - 
    static void Bin2Hex(const void* binary, size_t binary_len, char* hex);
    static std::string Bin2Hex(const void* binary, size_t binary_len);

    // @brief Convert a hex format buffer to binary
    // @param[in] hex - 
    // @param[in] hex_len - 
    // @param[in] binary - At least with length of hex_len/2
    // @return bool - 
    static bool Hex2Bin(const char* hex, const size_t hex_len, void* binary);
    static std::string Hex2Bin(const char* hex, const size_t hex_len);

    static int DehexChar(char c);
    static bool IsValid(const char* hexmd5, size_t hexmd5_length);
    static bool IsValid(const std::string& hexmd5);
private:
    PHP_MD5_CTX   md5_ctx;
};


inline void MD5::Finalizeh(char hex33[33]) {
    unsigned char binary16_digest[kBinDigestLength];
    PHP_MD5Final(binary16_digest, &md5_ctx);
    Bin2Hex(binary16_digest, kBinDigestLength, hex33);
}

inline void MD5::Sum(const void* data, size_t data_len, unsigned char binary16_digest[16]) {
    PHP_MD5_CTX ctx;
    PHP_MD5Init(&ctx);
    PHP_MD5Update(&ctx, (const unsigned char*)data, data_len);
    PHP_MD5Final(binary16_digest, &ctx);
}

inline std::string MD5::Sum(const std::string& d) {
    return Sum(d.data(), d.size());
}

inline void MD5::Sumh(const void* data, size_t data_len, char hex33[/*33*/]) {
    PHP_MD5_CTX ctx;
    PHP_MD5Init(&ctx);
    PHP_MD5Update(&ctx, (const unsigned char*)data, data_len);
    unsigned char* binary = reinterpret_cast<unsigned char*>(hex33 + kBinDigestLength);
    PHP_MD5Final(binary, &ctx);
    Bin2Hex(binary, kBinDigestLength, hex33);
}

inline std::string MD5::Sumh(const std::string& d) {
    return Sumh(d.data(), d.size());
}

inline std::string MD5::Sum(const void* data, size_t data_len) {
    std::string binary16_digest;
    binary16_digest.resize(16);
    Sum(data, data_len, reinterpret_cast<unsigned char*>(&binary16_digest[0]));
    return binary16_digest;
}

inline std::string MD5::Sumh(const void* data, size_t data_len) {
    std::string hex33;
    hex33.resize(kHexDigestLength + 1);
    Sumh(data, data_len, &hex33[0]);
    hex33.resize(kHexDigestLength);
    return hex33;
}

inline void MD5::Bin2Hex(const void* binary, size_t binary_len, char* hex) {
    register int m = 0;
    const uint8_t* ubinary = reinterpret_cast<const uint8_t*>(binary);

    for (size_t i = 0; i < binary_len; ++i) {
        m = (ubinary[i] & 0xF0) >> 4;

        if (m <= 9) {
            hex[i << 1] = m + '0';
        } else {
            hex[i << 1] = m + 'a' - 10;
        }

        m = (ubinary[i]) & 0x0F;

        if (m <= 9) {
            hex[(i << 1) + 1] = m + '0';
        } else {
            hex[(i << 1) + 1] = m + 'a' - 10;
        }
    }

    hex[binary_len << 1] = '\0';
}

inline std::string MD5::Bin2Hex(const void* binary, size_t binary_len) {
    std::string r;
    r.resize(binary_len << 1);
    Bin2Hex(binary, binary_len, &r[0]);
    return r;
}

inline bool MD5::Hex2Bin(const char* hex, const size_t hex_len, void* binary) {
    register int m = 0;
    uint8_t* ubinary = reinterpret_cast<uint8_t*>(binary);
    size_t i = 0;
    size_t hex_len_1 = hex_len - 1;

    while (i < hex_len_1) {
        m = DehexChar(hex[i]);

        if (m < 0) {
            return false;
        }

        ubinary[i >> 1] = m << 4;

        m = DehexChar(hex[i + 1]);

        if (m < 0) {
            return false;
        }

        ubinary[i >> 1] |= m;

        i += 2;
    }

    return true;
}

inline std::string MD5::Hex2Bin(const char* hex, const size_t hex_len) {
    std::string r;
    r.resize(hex_len >> 1);
    Hex2Bin(hex, hex_len, &r[0]);
    return r;
}

inline int MD5::DehexChar(char c) {
    static int hex2dec_table[256] = {
        /*       0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F */
        /* 0 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 1 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 2 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 3 */  0, 1, 2, 3,  4, 5, 6, 7,  8, 9,-1,-1, -1,-1,-1,-1,

        /* 4 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 5 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 6 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 7 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,

        /* 8 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 9 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* A */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* B */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,

        /* C */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* D */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* E */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* F */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1
    };

    return hex2dec_table[(int)c];
}

inline bool MD5::IsValid(const char* hexmd5, size_t hexmd5_length) {
    if (hexmd5_length > 32 || hexmd5_length == 0) {
        return false;
    }

    static const char valid_chars[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    for (size_t i = 0; i < hexmd5_length; ++i) {
        if (valid_chars[(int)hexmd5[i]] == 0) {
            return false;
        }
    }

    return true;
}

inline bool MD5::IsValid(const std::string& hexmd5) {
    return IsValid(hexmd5.c_str(), hexmd5.length());
}

} // simcc
