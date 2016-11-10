#include <assert.h>

namespace simcc {

// Template class for creating single-instance global classes. 
// 
// Usage:
//  <code>
//      // log_manager.h
//      class LogManager : public Singleton<LogManager>
//      {}
// 
//      // log_manager.cpp
//      template<> LogManager* Singleton<LogManager>::instance_ = NULL;
//  </code>
// 

template <typename T>
class Singleton {
public:
    static T* GetInstance() {
        assert(instance_);
        return instance_;
    }

protected:
    Singleton() {
        assert(!instance_);
        instance_ = static_cast<T*>(this);
    }

    ~Singleton() {
        instance_ = NULL;
    }

protected:
    static T* instance_;
};

}

