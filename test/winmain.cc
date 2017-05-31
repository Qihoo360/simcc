#include "test_common.h"

namespace {
#ifdef _WIN32
struct OnApp {
    ~OnApp() {
        system("pause");
    }
} __s_onexit_pause;
#endif
}

// main function is defined on gtest_main.cc