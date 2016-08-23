#pragma once

#include "wcpp/inner_pre.h"
#include "wcpp/ref_object.h"
#include "wcpp/timestamp.h"

#include <mutex>
#include <map>

namespace wcpp {

class WCPP_EXPORT DoubleBuffering {
public:
    class WCPP_EXPORT Target : public RefObject {
    public:
        virtual bool Initialize(const string& conf) = 0;
        virtual std::string MD5() const { return ""; }
        virtual ~Target() {}
    };
    typedef RefPtr<Target> TargetPtr;
    typedef std::function<TargetPtr ()> TargetCreator;

    DoubleBuffering(TargetCreator f);

    bool Reload(const string& conf);

    std::string MD5() const;

    Timestamp reload_time() const {
        return reload_time_;
    }

    TargetPtr Get() const;

private:
    mutable std::mutex mutex_;
    TargetPtr current_;
    TargetCreator creator_;
    Timestamp reload_time_;
};

typedef std::shared_ptr<DoubleBuffering> DoubleBufferingPtr;

class WCPP_EXPORT DoubleBufferingManager {
public:
    DoubleBufferingManager();

    bool Add(const std::string& name,
             const std::string& conf,
             DoubleBuffering::TargetCreator f);

    DoubleBuffering::TargetPtr Get(const std::string& name) const;
    bool Reload(const std::string& name, const std::string& conf);

private:
    std::map<std::string/*name*/, DoubleBufferingPtr> dbufs_;
    mutable std::mutex mutex_;
};

}
