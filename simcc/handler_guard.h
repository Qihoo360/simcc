#pragma once

#include <functional>

namespace simcc {

class HandlerGuard {
public:
    typedef std::function<void()> Handler;

public:
    HandlerGuard(Handler h)
        : cb_(h), execute_(true) {}

    ~HandlerGuard() {
        if (execute_) {
            cb_();
        }
    }

    void set_execute(bool v) { execute_ = v; }
private:
    Handler cb_;
    bool execute_;
};

}


