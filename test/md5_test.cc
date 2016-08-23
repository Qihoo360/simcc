
#include "test_common.h"
#include "wcpp/random.h"
#include "wcpp/data_stream.h"
#include "wcpp/misc/md5.h"
#include "wcpp/misc/php_md5.h"

#include <iostream>



// MD5.CC - source code for the C++/object oriented translation and
//          modification of MD5.

// Translation and modification (c) 1995 by Mordechai T. Abzug

// This translation/ modification is provided "as is," without express or
// implied warranty of any kind.

// The translator/ modifier does not claim (1) that MD5 will do what you think
// it does; (2) that this translation/ modification is accurate; or (3) that
// this software is "merchantible."  (Language for this disclaimer partially
// copied from the disclaimer below).

/* based on:

MD5.H - header file for MD5C.C
MDDRIVER.C - test driver for MD2, MD4 and MD5

Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.

*/

namespace rsacc {
class MD5 {
public:
    enum {
        MD5_RAW_BIN_DIGEST_LEN = 16, // the length of digest as a 16-byte ubinary array
        MD5_HEX_DIGEST_LEN = 32, // the length of digest as a 32-byte ASCII-hex string, not include the terminating null-character
    };
public:
    // methods for controlled operation:
    MD5();  // simple initializer

    // Constructors for special circumstances.
    // All these constructors finalize the MD5 context.
    MD5(const void* str, size_t input_length); // digest string, finalize
    MD5(std::istream& stream);       // digest stream, finalize
    MD5(FILE* file);            // digest file, close, finalize
    MD5(std::ifstream& stream);      // digest stream, close, finalize

    void update(const void* input, size_t input_length);
    void update(std::istream& stream);
    void update(FILE* file);
    void update(std::ifstream& stream);
    void finalize();

    //! Method to acquire finalized result which is digest as a 16-byte ubinary array
    //! \param[out] unsigned char * binary16md5, the result stored here, it MUST BE at least 16 bytes long
    //! \return true if digest the input source data successfully and digest data is stored in output
    bool getRawDigest(unsigned char binary16md5[/*16*/]);    // digest as a 16-byte ubinary array

    //! Method to acquire finalized result which is digest as a 33-byte ASCII-hex string
    //! \param[out] char* output, the result stored here, output MUST BE at least 33 bytes long
    //! \return true if digest the input source data successfully and digest data is stored in output
    bool getHexDigest(char hex33md5[/*33*/]);   //digest as a 33-byte ASCII-hex string, with the terminating null-character


    //! Get the hex 33 bytes md5 string from a 16 bytes long ubinary md5 string
    static void getHex33Str(const unsigned char rawmd5[/*16*/], char hex33[/*33*/]) {
        convertBinary16ToHex32(rawmd5, hex33);
    }

    //! Return a random hex md5, 33 bytes md5 string (including the terminating char \0)
    static std::string genHexMD5(const size_t random_data_len = 1024);

    //! Return a random ubinary md5, 16 bytes md5 string
    static std::string genRawMD5(const size_t random_data_len = 1024);

    //! Return the md5 digest of <code>data</code>
    static void getHexMD5(const void* data, size_t data_len, char hex33md5[/*33*/]);
    static void hexdigest(const void* data, size_t data_len, char hex33md5[/*33*/]) {
        getHexMD5(data, data_len, hex33md5);
    }
    static std::string hexdigest(const void* data, size_t data_len) {
        return getHexMD5(data, data_len);
    }
    static std::string getHexMD5(const void* data, size_t data_len) {
        char hex33md5[33] = {};
        getHexMD5(data, data_len, hex33md5);
        return hex33md5;
    }

    //! Return the md5 digest of <code>data</code>
    //! \param[out] unsigned char * binary16md5, the result stored here, it MUST BE at least 16 bytes long
    static void getRawMD5(const void* data, size_t data_len, unsigned char binary16md5[/*16*/]);

    static std::string getRawMD5(const void* data, size_t data_len) {
        unsigned char binary16md5[16] = {};
        getRawMD5(data, data_len, binary16md5);
        return std::string((char*)binary16md5, sizeof(binary16md5));
    }

    static bool isValidMD5(const std::string& hexmd5) {
        return isValidMD5(hexmd5.c_str(), hexmd5.length());
    }


    static void convertBinary16ToHex32(const uint8_t binary16[/* 16 */], char hex33[/*33*/]) {
        int m = 0;

        for (size_t i = 0; i < 16; ++i) {
            m = (binary16[i] & 0xF0) >> 4;

            if (m <= 9) {
                hex33[i << 1] = m + '0';
            } else {
                hex33[i << 1] = m + 'a' - 10;
            }

            m = (binary16[i]) & 0x0F;

            if (m <= 9) {
                hex33[(i << 1) + 1] = m + '0';
            } else {
                hex33[(i << 1) + 1] = m + 'a' - 10;
            }
        }

        hex33[32] = '\0';
    }

    static std::string convertBinary16ToHex32(const uint8_t binary16[/* 16 */]) {
        char hex33[33] = {};
        convertBinary16ToHex32(binary16, hex33);
        return std::string(hex33, 32);
    }

    static int dehexchar(char c) {
        if (c >= '0' && c <= '9') {
            return c - '0';
        }

        if (c >= 'A' && c <= 'F') {
            return c - ('A' - 10);
        }

        if (c >= 'a' && c <= 'f') {
            return c - ('a' - 10);
        }

        return -1;
    }

    static std::string convertHex32ToBinary16(const char hex33[/*32*/], const size_t hex33_len = MD5_HEX_DIGEST_LEN) {
        uint8_t binary16[16] = {};
        convertHex32ToBinary16(hex33, hex33_len, binary16);
        return std::string((char*)binary16, sizeof(binary16));
    }

    static void convertHex32ToBinary16(const char hex33[/*32*/], const size_t hex33_len, uint8_t binary16[/* 16 */]) {
        int m = 0;
        size_t i = 0;

        while (i < hex33_len) {
            m = dehexchar(hex33[i]);

            if (m < 0) {
                break;
            }

            binary16[i >> 1] = m << 4;

            if (i + 1 >= hex33_len) {
                break;
            }

            m = dehexchar(hex33[i + 1]);

            if (m < 0) {
                break;
            }

            binary16[i >> 1] |= m;

            i += 2;
        }
    }

    static bool isValidMD5(const char* hexmd5, size_t hexmd5_length) {
        if (hexmd5_length > 32 || hexmd5_length == 0) {
            return false;
        }

        static const char validmd5chs[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

        for (size_t i = 0; i < hexmd5_length; ++i) {
            if (validmd5chs[(int)hexmd5[i]] == 0) {
                return false;
            }
        }

        return true;
    }
    //friend std::ostream& operator<< (std::ostream&, const MD5& context);

private:

    // first, some types:
    typedef uint32_t uint4; // assumes integer is 4 words long
    typedef uint16_t uint2; // assumes short integer is 2 words long
    typedef uint8_t uint1; // assumes char is 1 word long

    // next, the private data:
    uint4 state[4];
    uint4 count[2];     // number of *bits*, mod 2^64
    uint1 buffer[64];   // input buffer
    uint1 digest[16];
    uint1 finalized;

    // last, the private methods, mostly static:
    void init();                            // called by all constructors
    void transform(uint1* buffer);          // does the real update work.  Note
    // that length is implied to be 64.

    static void encode(uint1* dest, uint4* src, uint4 length);
    static void decode(uint4* dest, uint1* src, uint4 length);
    static void memcpy(uint1* dest, uint1* src, uint4 length);
    static void memset(uint1* start, uint1 val, uint4 length);

    static inline uint4  rotate_left(uint4 x, uint4 n);
    static inline uint4  F(uint4 x, uint4 y, uint4 z);
    static inline uint4  G(uint4 x, uint4 y, uint4 z);
    static inline uint4  H(uint4 x, uint4 y, uint4 z);
    static inline uint4  I(uint4 x, uint4 y, uint4 z);
    static inline void   FF(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x,
                            uint4 s, uint4 ac);
    static inline void   GG(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x,
                            uint4 s, uint4 ac);
    static inline void   HH(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x,
                            uint4 s, uint4 ac);
    static inline void   II(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x,
                            uint4 s, uint4 ac);

};


// ROTATE_LEFT rotates x left n bits.

inline unsigned int MD5::rotate_left(uint4 x, uint4 n) {
    return (x << n) | (x >> (32 - n))  ;
}




// F, G, H and I are basic MD5 functions.

inline unsigned int MD5::F(uint4 x, uint4 y, uint4 z) {
    return (x & y) | (~x & z);
}

inline unsigned int MD5::G(uint4 x, uint4 y, uint4 z) {
    return (x & z) | (y & ~z);
}

inline unsigned int MD5::H(uint4 x, uint4 y, uint4 z) {
    return x ^ y ^ z;
}

inline unsigned int MD5::I(uint4 x, uint4 y, uint4 z) {
    return y ^ (x | ~z);
}



// FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
// Rotation is separate from addition to prevent recomputation.


inline void MD5::FF(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x,
                    uint4  s, uint4 ac) {
    a += F(b, c, d) + x + ac;
    a = rotate_left(a, s) + b;
}

inline void MD5::GG(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x,
                    uint4 s, uint4 ac) {
    a += G(b, c, d) + x + ac;
    a = rotate_left(a, s) + b;
}

inline void MD5::HH(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x,
                    uint4 s, uint4 ac) {
    a += H(b, c, d) + x + ac;
    a = rotate_left(a, s) + b;
}

inline void MD5::II(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x,
                    uint4 s, uint4 ac) {
    a += I(b, c, d) + x + ac;
    a = rotate_left(a, s) + b;
}
}

namespace rsacc {

MD5::MD5() {
    init();
}


MD5::MD5(FILE* file) {

    init();  // must be called be all constructors
    update(file);
    finalize();
}




MD5::MD5(std::istream& stream) {

    init();  // must called by all constructors
    update(stream);
    finalize();
}



MD5::MD5(std::ifstream& stream) {

    init();  // must called by all constructors
    update(stream);
    finalize();
}

MD5::MD5(const void* str, size_t input_length) {
    init();  // must called by all constructors
    update(str, input_length);
    finalize();
}




// MD5 block update operation. Continues an MD5 message-digest
// operation, processing another message block, and updating the
// context.

void MD5::update(const void* input_v, size_t input_length) {

    uint1* input = (uint1*)const_cast<void*>(input_v);

    uint4 input_index, buffer_index;
    uint4 buffer_space;                // how much space is left in buffer

    if (finalized) {  // so we can't update!
        std::cerr << "MD5::update:  Can't update a finalized digest!" << std::endl;
        return;
    }

    // Compute number of bytes mod 64
    buffer_index = (unsigned int)((count[0] >> 3) & 0x3F);

    // Update number of bits
    if ((count[0] += ((uint4)input_length << 3)) < ((uint4)input_length << 3)) {
        count[1]++;
    }

    count[1] += ((uint4)input_length >> 29);


    buffer_space = 64 - buffer_index;  // how much space is left in buffer

    // Transform as many times as possible.
    if (input_length >= buffer_space) { // ie. we have enough to fill the buffer
        // fill the rest of the buffer and transform
        memcpy(buffer + buffer_index, input, buffer_space);
        transform(buffer);

        // now, transform each 64-byte piece of the input, bypassing the buffer
        for (input_index = buffer_space; input_index + 63 < input_length;
                input_index += 64) {
            transform(input + input_index);
        }

        buffer_index = 0;  // so we can buffer remaining
    } else {
        input_index = 0;    // so we can buffer the whole input
    }


    // and here we do the buffering:
    memcpy(buffer + buffer_index, input + input_index, input_length - input_index);
}



// MD5 update for files.
// Like above, except that it works on files (and uses above as a primitive.)

void MD5::update(FILE* file) {

    unsigned char buf[1024];
    int len;

    while ((len = fread(buf, 1, 1024, file)) != 0) {
        update(buf, len);
    }

    fclose(file);

}






// MD5 update for istreams.
// Like update for files; see above.

void MD5::update(std::istream& stream) {

    /*unsigned*/ char buf[1024];
    int len;

    while (stream.good()) {
        stream.read(buf, 1024); // note that return value of read is unusable.
        len = stream.gcount();
        update((uint1*)buf, len);
    }

}






// MD5 update for ifstreams.
// Like update for files; see above.

void MD5::update(std::ifstream& stream) {

    /*unsigned*/ char buf[1024];
    int len;

    while (stream.good()) {
        stream.read(buf, 1024); // note that return value of read is unusable.
        len = stream.gcount();
        update((uint1*)buf, len);
    }

}






// MD5 finalization. Ends an MD5 message-digest operation, writing the
// the message digest and zeroizing the context.


void MD5::finalize() {

    unsigned char bits[8];
    unsigned int index, padLen;
    static uint1 PADDING[64] = {
        0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    if (finalized) {
        std::cerr << "MD5::finalize:  Already finalized this digest!" << std::endl;
        return;
    }

    // Save number of bits
    encode(bits, count, 8);

    // Pad out to 56 mod 64.
    index = (uint4)((count[0] >> 3) & 0x3f);
    padLen = (index < 56) ? (56 - index) : (120 - index);
    update(PADDING, padLen);

    // Append length (before padding)
    update(bits, 8);

    // Store state in digest
    encode(digest, state, 16);

    // Zeroize sensitive information
    memset(buffer, 0, sizeof(*buffer));

    finalized = 1;

}


bool MD5::getRawDigest(unsigned char* outputbuf) {

    if (!finalized) {
        std::cerr << "MD5::getRawDigest:  Can't get digest if you haven't " <<
                  "finalized the digest!" << std::endl;
        return false;
    }

    memcpy(outputbuf, digest, 16);
    return true;
}



bool MD5::getHexDigest(char* outputbuf) {
    if (!finalized) {
        std::cerr << "MD5::hex_digest:  Can't get digest if you haven't " <<
                  "finalized the digest!" << std::endl;
        return false;
    }

    for (int i = 0; i < 16; i++) {
        sprintf(outputbuf + i * 2, "%02x", digest[i]);
    }

    outputbuf[32] = '\0';
    return true;
}

std::string MD5::genRawMD5(const size_t random_data_len) {
    std::string data;
    data.resize(random_data_len);

    for (size_t i = 0; i < random_data_len; ++i) {
        data[i] = rand() % 255;
    }

    return MD5::getRawMD5(reinterpret_cast<const void*>(data.data()), data.length());
}

std::string MD5::genHexMD5(const size_t random_data_len) {
    std::string data;
    data.resize(random_data_len);

    for (size_t i = 0; i < random_data_len; ++i) {
        data[i] = rand() % 255;
    }

    return MD5::getHexMD5(reinterpret_cast<const void*>(data.data()), data.length());
}


void MD5::getHexMD5(const void* data, size_t data_len, char hex33md5[/*33*/]) {
    MD5 md5((unsigned char*)const_cast<void*>(data), (unsigned int)data_len);
    md5.getHexDigest(hex33md5);
}


void MD5::getRawMD5(const void* data, size_t data_len, unsigned char binary16md5[/*16*/]) {
    MD5 md5((unsigned char*)const_cast<void*>(data), (unsigned int)data_len);
    md5.getRawDigest(binary16md5);
}

//     std::ostream& operator<<(std::ostream &stream, const MD5& context){
//
//         stream << context.getHexDigest();
//         return stream;
//     }




// PRIVATE METHODS:



void MD5::init() {
    finalized = 0;  // we just started!

    // Nothing counted, so count=0
    count[0] = 0;
    count[1] = 0;

    // Load magic initialization constants.
    state[0] = 0x67452301;
    state[1] = 0xefcdab89;
    state[2] = 0x98badcfe;
    state[3] = 0x10325476;
}



// Constants for MD5Transform routine.
// Although we could use C++ style constants, defines are actually better,
// since they let us easily evade scope clashes.

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21




// MD5 basic transformation. Transforms state based on block.
void MD5::transform(uint1 block[64]) {

    uint4 a = state[0], b = state[1], c = state[2], d = state[3], x[16];

    decode(x, block, 64);

    assert(!finalized);  // not just a user error, since the method is private

    /* Round 1 */
    FF(a, b, c, d, x[0], S11, 0xd76aa478); /* 1 */
    FF(d, a, b, c, x[1], S12, 0xe8c7b756); /* 2 */
    FF(c, d, a, b, x[2], S13, 0x242070db); /* 3 */
    FF(b, c, d, a, x[3], S14, 0xc1bdceee); /* 4 */
    FF(a, b, c, d, x[4], S11, 0xf57c0faf); /* 5 */
    FF(d, a, b, c, x[5], S12, 0x4787c62a); /* 6 */
    FF(c, d, a, b, x[6], S13, 0xa8304613); /* 7 */
    FF(b, c, d, a, x[7], S14, 0xfd469501); /* 8 */
    FF(a, b, c, d, x[8], S11, 0x698098d8); /* 9 */
    FF(d, a, b, c, x[9], S12, 0x8b44f7af); /* 10 */
    FF(c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
    FF(b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
    FF(a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
    FF(d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
    FF(c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
    FF(b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

    /* Round 2 */
    GG(a, b, c, d, x[1], S21, 0xf61e2562); /* 17 */
    GG(d, a, b, c, x[6], S22, 0xc040b340); /* 18 */
    GG(c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
    GG(b, c, d, a, x[0], S24, 0xe9b6c7aa); /* 20 */
    GG(a, b, c, d, x[5], S21, 0xd62f105d); /* 21 */
    GG(d, a, b, c, x[10], S22, 0x2441453); /* 22 */
    GG(c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
    GG(b, c, d, a, x[4], S24, 0xe7d3fbc8); /* 24 */
    GG(a, b, c, d, x[9], S21, 0x21e1cde6); /* 25 */
    GG(d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
    GG(c, d, a, b, x[3], S23, 0xf4d50d87); /* 27 */
    GG(b, c, d, a, x[8], S24, 0x455a14ed); /* 28 */
    GG(a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
    GG(d, a, b, c, x[2], S22, 0xfcefa3f8); /* 30 */
    GG(c, d, a, b, x[7], S23, 0x676f02d9); /* 31 */
    GG(b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

    /* Round 3 */
    HH(a, b, c, d, x[5], S31, 0xfffa3942); /* 33 */
    HH(d, a, b, c, x[8], S32, 0x8771f681); /* 34 */
    HH(c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
    HH(b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
    HH(a, b, c, d, x[1], S31, 0xa4beea44); /* 37 */
    HH(d, a, b, c, x[4], S32, 0x4bdecfa9); /* 38 */
    HH(c, d, a, b, x[7], S33, 0xf6bb4b60); /* 39 */
    HH(b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
    HH(a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
    HH(d, a, b, c, x[0], S32, 0xeaa127fa); /* 42 */
    HH(c, d, a, b, x[3], S33, 0xd4ef3085); /* 43 */
    HH(b, c, d, a, x[6], S34, 0x4881d05); /* 44 */
    HH(a, b, c, d, x[9], S31, 0xd9d4d039); /* 45 */
    HH(d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
    HH(c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
    HH(b, c, d, a, x[2], S34, 0xc4ac5665); /* 48 */

    /* Round 4 */
    II(a, b, c, d, x[0], S41, 0xf4292244); /* 49 */
    II(d, a, b, c, x[7], S42, 0x432aff97); /* 50 */
    II(c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
    II(b, c, d, a, x[5], S44, 0xfc93a039); /* 52 */
    II(a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
    II(d, a, b, c, x[3], S42, 0x8f0ccc92); /* 54 */
    II(c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
    II(b, c, d, a, x[1], S44, 0x85845dd1); /* 56 */
    II(a, b, c, d, x[8], S41, 0x6fa87e4f); /* 57 */
    II(d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
    II(c, d, a, b, x[6], S43, 0xa3014314); /* 59 */
    II(b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
    II(a, b, c, d, x[4], S41, 0xf7537e82); /* 61 */
    II(d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
    II(c, d, a, b, x[2], S43, 0x2ad7d2bb); /* 63 */
    II(b, c, d, a, x[9], S44, 0xeb86d391); /* 64 */

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;

    // Zeroize sensitive information.
    memset((uint1*)x, 0, sizeof(x));

}



// Encodes input (UINT4) into output (unsigned char). Assumes len is
// a multiple of 4.
void MD5::encode(uint1* output, uint4* input, uint4 len) {

    unsigned int i, j;

    for (i = 0, j = 0; j < len; i++, j += 4) {
        output[j] = (uint1)(input[i] & 0xff);
        output[j + 1] = (uint1)((input[i] >> 8) & 0xff);
        output[j + 2] = (uint1)((input[i] >> 16) & 0xff);
        output[j + 3] = (uint1)((input[i] >> 24) & 0xff);
    }
}

// Decodes input (unsigned char) into output (UINT4). Assumes len is
// a multiple of 4.
void MD5::decode(uint4* output, uint1* input, uint4 len) {

    unsigned int i, j;

    for (i = 0, j = 0; j < len; i++, j += 4)
        output[i] = ((uint4)input[j]) | (((uint4)input[j + 1]) << 8) |
                    (((uint4)input[j + 2]) << 16) | (((uint4)input[j + 3]) << 24);
}

// Note: Replace "for loop" with standard memcpy if possible.
void MD5::memcpy(uint1* output, uint1* input, uint4 len) {

    unsigned int i;

    for (i = 0; i < len; i++) {
        output[i] = input[i];
    }
}

// Note: Replace "for loop" with standard memset if possible.
void MD5::memset(uint1* output, uint1 value, uint4 len) {

    unsigned int i;

    for (i = 0; i < len; i++) {
        output[i] = value;
    }
}
}


namespace
{
    void php_md5( const void* data, size_t data_len, uint8_t binary16[/*16*/] )
    {
        PHP_MD5_CTX ctx;
        PHP_MD5Init(&ctx);
        PHP_MD5Update(&ctx, (const unsigned char*)(data), (unsigned int)data_len);
        PHP_MD5Final((unsigned char*)binary16, &ctx);
    }
}

TEST_UNIT(md5_test_1)
{
    uint8_t r1[16];
    uint8_t r2[16];
    uint8_t r3[16];
    const char* data     = "ja;slfja;lsdjfpu-2341293ursadklf;asnvzaxcvklahpsdfp-98ur4-23hf;asdfja";
    size_t      data_len = strlen(data);
    rsacc::MD5::getRawMD5(data, data_len, r1);
    php_md5(data, data_len, r2);
    H_TEST_ASSERT(memcmp(r1, r2, sizeof(r1)) == 0);
    wcpp::MD5::Sum(data, data_len, r3);
    wcpp::MD5::Sum(data, data_len, r3);
    H_TEST_ASSERT(memcmp(r1, r3, sizeof(r1)) == 0);

    std::string s1 = rsacc::MD5::hexdigest(data, data_len);
    std::string s2 = wcpp::MD5::Sumh(data, data_len);
    H_TEST_ASSERT(s1 == s2);
}


namespace {
    struct MID
    {
        union
        {
            struct
            {
                uint64_t l_;
                uint64_t h_;
            };
            unsigned char d_[16];
        };

        /*
        MID(const MID& r)
        {
        memcpy(d_, r.d_, sizeof(d_));
        }

        MID& operator=(const MID& r) 
        {
        memcpy(d_, r.d_, sizeof(d_));
        return *this;
        }*/
    };
}

TEST_UNIT(md5_convert_hex_binary_test)
{
#if 0
    const char* hexmd5 = "c5928f43965a3f48d2dcca2e60d6f195";
    MID mid;
    wcpp::MD5::hex2bin(hexmd5, 32, mid.d_);
    std::cout << "         hexmd5=" << hexmd5 << std::endl;
    std::cout << " binarymd5.low8=" << mid.l_ << std::endl;
    std::cout << "binarymd5.high8=" << mid.h_ << std::endl;


    hexmd5 = "361ec4d456066efa11886a43ebf7f3f1";
    wcpp::MD5::hex2bin(hexmd5, 32, mid.d_);
    std::cout << "         hexmd5=" << hexmd5 << std::endl;
    std::cout << " binarymd5.low8=" << mid.l_ << std::endl;
    std::cout << "binarymd5.high8=" << mid.h_ << std::endl;
#endif
}

TEST_UNIT(md5_test2)
{
    wcpp::DataStream ds;
    ds  << "abcdaljdslfjalsdjfqpoiuerpoqiuewr9127834012384-08fjsd;lfjas;ldfa'sdfp"
        << "['qpoweirqwuier-01984-2307419273sdajf;aj  jas;ldfjapoifuq987401234dsjfa;ljasjd;flasjdf;"
        << "ljasd;fjasdfoihasdpoifhpasoidfpqwoeiurpqowief;lksdj;kxncv/zxcnv/,zxcv;wqejrpoweiur01239874"
        << "012987502135861023987098uaesfpoisjd;fajsd;vlckxncv;zlkvj'sadpf]sadfsdfcxlvkaj;sdjifasdf";

    {
        char result[33] ={'\0'} ;

        //new MD5
        {
            wcpp::MD5 md5;
            md5.Update( (unsigned char *)ds.GetCache(), ds.size() );
            md5.Finalizeh(result);
            //std::cout << result << std::endl;
            //H_ASSERT( strncmp( "127fe73c2db3436a948b1c41469dc9e0", (const char*)result, 32 ) == 0 );
        }

        {
            char result1[33] = {0};
            {
                wcpp::MD5 md5;
                for ( unsigned int i = 0; i < ds.size(); ++i )
                {
                    md5.Update( ((unsigned char*)ds.GetCache()) + i, 1 );
                }
                md5.Finalizeh(result1);
            }

            char result8[33] = {0};
            {
                wcpp::MD5 md5;
                const int UNIT_LEN = 8;
                int oct = ds.size()/UNIT_LEN;
                for ( int i = 0; i < oct; ++i )
                {
                    md5.Update( ((unsigned char*)ds.GetCache()) + i * UNIT_LEN, UNIT_LEN );
                }

                int remain = ds.size() % UNIT_LEN;
                if ( remain != 0 )
                {
                    md5.Update( ((unsigned char*)ds.GetCache()) + ds.size() - remain, remain );
                }

                md5.Finalizeh(result8);
            }


            H_TEST_ASSERT( strcmp( result1, result8 ) == 0 );
            H_TEST_ASSERT( strcmp( result, result8 ) == 0 );
        }
    }
}
TEST_UNIT(md5_test3)
{
    std::string d = "abcdaljdslfjalsdjfqpoiuerpoqiuewr9127834012384-08fjsd;lfjas;ldfa'sdfp";
    std::string a = wcpp::MD5::Sum(d.data(), d.size());
    std::string b = wcpp::MD5::Sum(d);
    H_TEST_ASSERT( a == b );
}
TEST_UNIT(md5_test4)
{
    std::string d = "abcdaljdslfjalsdjfqpoiuerpoqiuewr9127834012384-08fjsd;lfjas;ldfa'sdfp";
    std::string a = wcpp::MD5::Sumh(d.data(), d.size());
    std::string b = wcpp::MD5::Sumh(d);
    H_TEST_ASSERT( a == b );
}

namespace {

    static int dehexchar( char c )
    {
        if ( c >= '0' && c <= '9' )
        {
            return c - '0';
        }

        if ( c >= 'A' && c <= 'F' )
        {
            return c - ( 'A' - 10 );
        }

        if ( c >= 'a' && c <= 'f' )
        {
            return c - ( 'a' - 10 );
        }

        return -1;
    }

#if 0
    static void convertHex32ToBinary16(const char hex33[/*32*/], const size_t hex33_len, uint8_t binary16[/* 16 */])
    {
        int m = 0;
        size_t i = 0;
        while (i < hex33_len)
        {
            m = dehexchar(hex33[i]);
            if (m < 0)
            {
                break;
            }
            binary16[i >> 1] = m << 4;

            if (i + 1 >= hex33_len)
            {
                break;
            }

            m = dehexchar(hex33[i+1]);
            if (m < 0)
            {
                break;
            }
            binary16[i >> 1] |= m;

            i += 2;
        }
    }
#endif

    void hex2bin_test(size_t randlen)
    {
        std::string rs = wcpp::RandomString::Rand(randlen);
        std::string hex = wcpp::MD5::Bin2Hex(rs.data(), rs.size());
        std::string bin = wcpp::MD5::Hex2Bin(hex.data(), hex.size());
        H_TEST_ASSERT(bin == rs);
    }
}

TEST_UNIT(dehexchar_test)
{
    std::string hex1 = "0123456789ABCDEF";
    for (size_t i = 0; i < hex1.size(); ++i)
    {
        H_TEST_ASSERT(wcpp::MD5::DehexChar(hex1[i]) == dehexchar(hex1[i]));
    }
    std::string hex2 = "0123456789abcdef;asjf;ijp984170293842phnfvzx.,nv;ih p98y340127341";
    for (size_t i = 0; i < hex2.size(); ++i)
    {
        H_TEST_ASSERT(wcpp::MD5::DehexChar(hex2[i]) == dehexchar(hex2[i]));
    }
}




TEST_UNIT(hex2dec_test)
{
    hex2bin_test(10);
    hex2bin_test(1024);
}


