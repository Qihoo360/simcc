#include "test_common.h"

#include <simcc/exp.h>
#include <simcc/handler_guard.h>

namespace {
    static void handler_guard_test(int *guard) {
        *guard = *guard + 1;
    }
}


TEST_UNIT(HandlerGuard_test) {
    int guard = 1;
    {
        simcc::HandlerGuard h(std::bind(&handler_guard_test, &guard));
    }
    H_TEST_ASSERT(guard == 2);
}


TEST_UNIT(HandlerGuard2_test) {
    int guard = 1;
    {
        simcc::HandlerGuard h(std::bind(&handler_guard_test, &guard));
        simcc::HandlerGuard g(std::bind(&handler_guard_test, &guard));
        simcc::HandlerGuard i(std::bind(&handler_guard_test, &guard));
        h.set_execute(false);
        g.set_execute(true);
        i.set_execute(true);
    }
    H_TEST_ASSERT(guard == 3);
}
