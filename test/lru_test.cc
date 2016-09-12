#include "test_common.h"

#include "simcc/misc/lru.h"


TEST_UNIT(testLRUCacheH4) {
    //LRU lru(2, 1000, LRU::SizeofFunctor(), LRU::DeleteFunctor());
    simcc::LRUCacheH4<std::string, int64_t> lru(2, 1000);
    lru.insert("key1", 1);
    H_TEST_ASSERT(lru.size() == 1);
    lru.insert("key2", 2);
    H_TEST_ASSERT(lru.size() == 2);
    lru.insert("key3", 3);
    H_TEST_ASSERT(lru.size() == 2);
    auto it = lru.find("key1");
    H_TEST_ASSERT(it == lru.end());
    lru.insert("key4", 4);
    H_TEST_ASSERT(lru.size() == 2);
    it = lru.find("key1");
    H_TEST_ASSERT(it == lru.end());
    it = lru.find("key2");
    lru.insert("key5", 5);
    H_TEST_ASSERT(lru.size() == 2);
    it = lru.find("key4");
    H_TEST_ASSERT(it != lru.end());
    it = lru.find("key5");
    H_TEST_ASSERT(it != lru.end());
    it = lru.find("key1");
    H_TEST_ASSERT(it == lru.end());
    it = lru.find("key2");
    H_TEST_ASSERT(it == lru.end());
    it = lru.find("key3");
    H_TEST_ASSERT(it == lru.end());
}


struct Sizeof : std::unary_function<int64_t, size_t> {
    size_t operator()(const int64_t& v) {
        return sizeof(std::string) + sizeof(int64_t);
    }
};

static int delete_count = 0;
static int64_t deleted_value = 0;
struct Deleter : std::unary_function<int64_t, void> {
    void operator()(const int64_t& v) {
        delete_count++;
        deleted_value = v;
    }
};


TEST_UNIT(testLRUCacheH4MemoryLRU) {
    delete_count = 0;
    {
        simcc::LRUCacheH4<std::string, int64_t, Sizeof, Deleter> lru(1000, 56, Sizeof(), Deleter());
        lru.insert("key1", 1);
        H_TEST_ASSERT(lru.size() == 1);
        H_TEST_ASSERT(delete_count == 0);
        lru.insert("key2", 2);
        H_TEST_ASSERT(lru.size() == 2);
        H_TEST_ASSERT(delete_count == 0);
        lru.insert("key3", 3);
        H_TEST_ASSERT(lru.size() == 2);
        H_TEST_ASSERT(delete_count == 1);
        H_TEST_ASSERT(deleted_value == 1);
        auto it = lru.find("key1");
        H_TEST_ASSERT(it == lru.end());
        lru.insert("key4", 4);
        H_TEST_ASSERT(lru.size() == 2);
        H_TEST_ASSERT(delete_count == 2);
        H_TEST_ASSERT(deleted_value == 2);
        it = lru.find("key1");
        H_TEST_ASSERT(it == lru.end());
        it = lru.find("key2");
        lru.insert("key5", 5);
        H_TEST_ASSERT(lru.size() == 2);
        H_TEST_ASSERT(delete_count == 3);
        H_TEST_ASSERT(deleted_value == 3);
        it = lru.find("key4");
        H_TEST_ASSERT(it != lru.end());
        it = lru.find("key5");
        H_TEST_ASSERT(it != lru.end());
        it = lru.find("key1");
        H_TEST_ASSERT(it == lru.end());
        it = lru.find("key2");
        H_TEST_ASSERT(it == lru.end());
        it = lru.find("key3");
        H_TEST_ASSERT(it == lru.end());
        H_TEST_ASSERT(delete_count == 3);
    }
    H_TEST_ASSERT(delete_count == 5);
    H_TEST_ASSERT(deleted_value == 5);
}

