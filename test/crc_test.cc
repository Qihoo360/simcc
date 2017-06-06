#include "test_common.h"
#include "simcc/misc/crc16.h"
#include "simcc/misc/crc32.h"

//TODO Fix this failed test case
//TEST_UNIT(CRC16_test1) {
//   std::string s = "hello";
//   uint16_t expected = 13558;
//   uint16_t crc16 = simcc::CRC16::Sum(s);
//   H_TEST_ASSERT(crc16 == expected);
//
//   s = "world";
//   expected = 61249;
//   crc16 = simcc::CRC16::Sum(s.data(), s.length());
//   H_TEST_ASSERT(crc16 == expected);
//}


TEST_UNIT(CRC32_test1) {
   std::string s = "hello";
   uint32_t crc = simcc::CRC32::Sum(s);
   H_TEST_ASSERT(crc == 907060870);
   s = "world";
   crc = simcc::CRC32::Sum(s);
   H_TEST_ASSERT(crc == 980881731);
}


