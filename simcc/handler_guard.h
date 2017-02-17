#pragma once

#include <functional>

namespace simcc {
class HandlerGuard {
public:
    typedef std::function<void()> ExcuteHandler;
public:
    HandlerGuard(ExcuteHandler h)
        : callback_(h), execute_(true) {}

    ~HandlerGuard() {
        if (execute_) {
            callback_();
        }
    }

    void set_execute(bool v) { execute_ = v; }
private:
    ExcuteHandler callback_;
    bool execute_;
};
}