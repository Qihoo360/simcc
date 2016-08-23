#include "double_buffering.h"

namespace wcpp {

DoubleBuffering::DoubleBuffering(TargetCreator f)
    : creator_(f), reload_time_(0) {}

bool DoubleBuffering::Reload(const string& conf) {
    auto t = creator_();
    if (!t->Initialize(conf)) {
        return false;
    }
    std::lock_guard<std::mutex> g(mutex_);
    current_ = t;
    reload_time_ = Timestamp::Now();
    return true;
}

DoubleBuffering::TargetPtr DoubleBuffering::Get() const {
    std::lock_guard<std::mutex> g(mutex_);
    auto t = current_;
    return t;
}

std::string DoubleBuffering::MD5() const {
    auto t = Get();
    return t->MD5();
}


DoubleBufferingManager::DoubleBufferingManager() {}

DoubleBuffering::TargetPtr DoubleBufferingManager::Get(const std::string& name) const {
    std::lock_guard<std::mutex> g(mutex_);
    auto it = dbufs_.find(name);
    if (it != dbufs_.end()) {
        return it->second->Get();
    }

    assert(false && "cannot find this kind of DoubleBuffering");
    return DoubleBuffering::TargetPtr();
}

bool DoubleBufferingManager::Add(const std::string& name,
                                 const std::string& conf,
                                 DoubleBuffering::TargetCreator f) {
    std::lock_guard<std::mutex> g(mutex_);
    DoubleBufferingPtr& db = dbufs_[name];
    db.reset(new DoubleBuffering(f));
    return db->Reload(conf);
}

bool DoubleBufferingManager::Reload(const std::string& name, const std::string& conf) {
    DoubleBufferingPtr& db = dbufs_[name];
    assert(db.get());
    return db->Reload(conf);
}

}