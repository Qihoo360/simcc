#include "crc32.h"

namespace simcc {

uint32_t CRC32::Reflect(uint32_t ref, char ch) {
    uint32_t i, value = 0;

    // swap bit0 and bit7, bit1 and bit6, and so on
    for (i = 1; i < uint32_t(ch + 1); i++) {
        if (ref & 1) {
            value |= 1 << (ch - i);
        }

        ref >>= 1;
    }

    return value;
}

void CRC32::InitTable(uint32_t* table) {
    uint32_t i, j;
    uint32_t crc, temp;
    (void)crc;

    for (i = 0; i < 256; i++) {
        temp = Reflect(i, 8);
        table[i] = temp << 24;

        for (j = 0; j < 8; j++) {
            uint32_t t1, t2;
            uint32_t flag = table[i] & 0x80000000;
            t1 = (table[i] << 1);

            if (flag == 0) {
                t2 = 0;
            } else {
                t2 = 0x04c11db7;
            }

            table[i] = t1 ^ t2 ;
        }

        crc = table[i];
        table[i] = Reflect(table[i], 32);
    }
}

unsigned int CRC32::Sum(const void* d, size_t len) {
    static bool init_table = true;
    static uint32_t table[256];

    if (init_table) {
        InitTable(table);
        init_table = false;
    }

    if (len == 0) {
        return 0;
    }

    unsigned int CRC = 0xFFFFFFFF;
    const uint8_t* p = (const uint8_t*)d;

    for (size_t i = 0; i < len; i++) {
        CRC = table[(CRC ^ (*(p + i))) & 0xff] ^ (CRC >> 8);
    }

    return ~CRC;
}

}

