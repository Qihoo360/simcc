#include "test_common.h"

#include <atomic>
#include <thread>
#include <wcpp/ref_object.h>

namespace {
static std::atomic<int> ref_count(0);
static std::atomic<int> release_count(0);

class MyRefCounted : public wcpp::RefObject {
public:
    MyRefCounted() {
        ref_count++;
    }
    ~MyRefCounted() {
        release_count++;
    }
private:
    char c;
};

}

TEST_UNIT(testRefObject) {
    const int size = 1000;
    auto f = [size]() {
        for (int i = 0; i < size; i++) {
            wcpp::RefPtr<MyRefCounted> r1 = new MyRefCounted;
            wcpp::RefPtr<MyRefCounted> r2 = r1;
            wcpp::RefPtr<MyRefCounted> r3 = r2;
            wcpp::RefPtr<MyRefCounted> r4 = r3;
            wcpp::RefPtr<MyRefCounted> r5 = r4;
            wcpp::RefPtr<MyRefCounted> r6 = r5;
            wcpp::RefPtr<MyRefCounted> r7 = r6;
            H_TEST_ASSERT(r1.use_count() == 7);
            H_TEST_ASSERT(r2.use_count() == 7);
            H_TEST_ASSERT(r3.use_count() == 7);
            H_TEST_ASSERT(r4.use_count() == 7);
            H_TEST_ASSERT(r5.use_count() == 7);
            H_TEST_ASSERT(r6.use_count() == 7);
            H_TEST_ASSERT(r7.use_count() == 7);
            r7.reset();
            H_TEST_ASSERT(r1.use_count() == 6);
            H_TEST_ASSERT(r2.use_count() == 6);
            H_TEST_ASSERT(r3.use_count() == 6);
            H_TEST_ASSERT(r4.use_count() == 6);
            H_TEST_ASSERT(r5.use_count() == 6);
            H_TEST_ASSERT(r6.use_count() == 6);
            r6.reset();
            H_TEST_ASSERT(r1.use_count() == 5);
            H_TEST_ASSERT(r2.use_count() == 5);
            H_TEST_ASSERT(r3.use_count() == 5);
            H_TEST_ASSERT(r4.use_count() == 5);
            H_TEST_ASSERT(r5.use_count() == 5);
        }
    };

    std::thread* threads[size];
    for (int i = 0; i < size; i++) {
        threads[i] = new std::thread(f);
    }

    for (int i = 0; i < size; i++) {
        threads[i]->join();
        delete threads[i];
    }

    H_TEST_ASSERT(ref_count == size*size);
    H_TEST_ASSERT(release_count == ref_count);
}

//#include <map>
//#include <string>
//#include <wcpp/inner_pre.h>
//
//namespace {
//class DgramFilter {
//public:
//private:
//    thread_local static std::map<std::string, std::string> attr_;
//};
//}