
#include "wcpp/exp.h"
#include "./test_common.h"
#include "wcpp/duration.h"
#include "wcpp/timestamp.h"

TEST_UNIT(testDuration) {
    wcpp::Duration d0(0);
    wcpp::Duration d1(1);
    wcpp::Duration d2(2);
    wcpp::Duration d3(2);
    H_TEST_ASSERT(d0 < d1);
    H_TEST_ASSERT(d1 < d2);
    H_TEST_ASSERT(d2 == d3);
    H_TEST_ASSERT(d0.IsZero());
    H_TEST_ASSERT(d0 <= d1);
    H_TEST_ASSERT(d1 <= d2);
    H_TEST_ASSERT(d2 <= d3);
    H_TEST_ASSERT(d2 >= d3);
    H_TEST_ASSERT(d1 > d0);
    H_TEST_ASSERT(d2 > d1);
    H_TEST_ASSERT(d1 >= d0);
    H_TEST_ASSERT(d2 >= d1);
}

//namespace {
//    static int64_t utcmicrosecond() {
//        struct timeval tv;
//        gettimeofday(&tv, NULL);
//        return (int64_t)(((int64_t)(tv.tv_sec)) * 1000000 + tv.tv_usec);
//    }
//}
//
//TEST_UNIT(testTimestamp) {
//    wcpp::Timestamp ts = wcpp::Timestamp::Now();
//    auto us = utcmicrosecond();
//    std::cout << "==================> us=" << us << " ts=" << ts.UnixNano() / 1000 << "\n";
//    H_TEST_ASSERT(ts.UnixNano() / 1000 == us);
//}

