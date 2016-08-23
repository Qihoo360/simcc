
#include "test_common.h"
#include "wcpp/misc/dgram_filter.h"

TEST_UNIT(testDgramFilter) {
    wcpp::DgramFilter<2> filter;
    uint32_t max_threshold = 3;
    size_t lru_max_item_count = 3;
    size_t lru_max_memery_size_mb = 1;
    uint32_t block_second = 5;
    filter.Initialize(true, max_threshold, lru_max_item_count, lru_max_memery_size_mb, block_second);
    filter.set_debug(true);

    std::string package1("package1");
    std::string package2("package2");
    std::string package3("package3");
    std::string package4("package4");
    std::string package5("package5");
    std::string ip("");


    // package1 添加3次，达到临界值
    H_TEST_ASSERT(!filter.IsNeedFilter(package1, ip));
    H_TEST_ASSERT(!filter.IsNeedFilter(package1, ip));
    H_TEST_ASSERT(!filter.IsNeedFilter(package1, ip));

    // package2 添加3次，达到临界值
    H_TEST_ASSERT(!filter.IsNeedFilter(package2, ip));
    H_TEST_ASSERT(!filter.IsNeedFilter(package2, ip));
    H_TEST_ASSERT(!filter.IsNeedFilter(package2, ip));

    H_TEST_ASSERT(filter.IsNeedFilter(package1, ip));
    H_TEST_ASSERT(filter.IsNeedFilter(package2, ip));

    H_TEST_ASSERT(!filter.IsNeedFilter(package3, ip));
    H_TEST_ASSERT(!filter.IsNeedFilter(package4, ip));
    H_TEST_ASSERT(!filter.IsNeedFilter(package5, ip));

    //package1,package2都被LRU淘汰了
    H_TEST_ASSERT(!filter.IsNeedFilter(package1, ip));
    H_TEST_ASSERT(!filter.IsNeedFilter(package1, ip));
    H_TEST_ASSERT(!filter.IsNeedFilter(package1, ip));

    H_TEST_ASSERT(filter.IsNeedFilter(package1, ip));

    H_TEST_ASSERT(!filter.IsNeedFilter(package2, ip));
    H_TEST_ASSERT(!filter.IsNeedFilter(package2, ip));

    //std::cout << "\n";

    //package3,package4都被LRU淘汰了
    H_TEST_ASSERT(!filter.IsNeedFilter(package5, ip));
    H_TEST_ASSERT(!filter.IsNeedFilter(package5, ip));
    H_TEST_ASSERT(filter.IsNeedFilter(package5, ip));
    H_TEST_ASSERT(filter.IsNeedFilter(package5, ip));
    H_TEST_ASSERT(filter.IsNeedFilter(package5, ip));


    //std::cout << "\n";
    usleep((block_second + 1) * 1000 * 1000);

    // block_second 时间已过，所有历史计数都清零

    H_TEST_ASSERT(!filter.IsNeedFilter(package5, ip));
    H_TEST_ASSERT(!filter.IsNeedFilter(package5, ip));
    H_TEST_ASSERT(!filter.IsNeedFilter(package5, ip));

    H_TEST_ASSERT(filter.IsNeedFilter(package5, ip));
    H_TEST_ASSERT(filter.IsNeedFilter(package5, ip));
    H_TEST_ASSERT(filter.IsNeedFilter(package5, ip));

    H_TEST_ASSERT(!filter.IsNeedFilter(package1, ip));
    H_TEST_ASSERT(!filter.IsNeedFilter(package1, ip));
    H_TEST_ASSERT(!filter.IsNeedFilter(package1, ip));
    H_TEST_ASSERT(filter.IsNeedFilter(package1, ip));

    H_TEST_ASSERT(!filter.IsNeedFilter(package2, ip));
    H_TEST_ASSERT(!filter.IsNeedFilter(package2, ip));
    H_TEST_ASSERT(!filter.IsNeedFilter(package2, ip));
    H_TEST_ASSERT(filter.IsNeedFilter(package2, ip));
}
