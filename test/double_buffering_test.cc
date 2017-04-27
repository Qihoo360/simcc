#include "test_common.h"

#include "simcc/misc/double_buffering.h"

namespace {
class UTTarget : public simcc::DoubleBuffering::Target {
public:
    UTTarget() {}

    bool Initialize(const std::string& c) {
        conf_ = c;
        return true;
    }

    const std::string& conf() const {
        return conf_;
    }
private:
    std::string conf_;
};
typedef simcc::RefPtr<UTTarget> UTTargetPTr;

simcc::DoubleBuffering::TargetPtr Creator() {
    return simcc::DoubleBuffering::TargetPtr(new UTTarget);
}
}

TEST_UNIT(testDoubleBuffering) {
    simcc::DoubleBuffering db(&Creator);
    static const char* s[] = {
        "1",
        "12",
        "123",
        "1234",
        "12345",
        "123456",
        "1234567",
        "12345678",
        "123456789",
        "12345678901",
        "123456789012",
        "1234567890123",
        "12345678901234",
        "123456789012345",
        "1234567890123456",
        "12345678901234567" };
    for (size_t i = 0; i < H_ARRAYSIZE(s); ++i) {
        H_TEST_ASSERT(db.Reload(s[i]));
        simcc::DoubleBuffering::TargetPtr tt = db.Get();
        UTTarget* t = static_cast<UTTarget*>(tt.get());
        H_TEST_ASSERT(t->conf() == s[i]);
        H_TEST_ASSERT(tt->RefCount() == t->RefCount());
        H_TEST_ASSERT(t->RefCount() == 2);

        UTTargetPTr ut = t;
        H_TEST_ASSERT(ut->RefCount() == 3);

        ut = NULL;
        H_TEST_ASSERT(t->RefCount() == 2);

        tt = NULL;
        H_TEST_ASSERT(t->RefCount() == 1);
    }
}


TEST_UNIT(testDoubleBufferingManager) {
    simcc::DoubleBufferingManager m;
    static const char* s[] = {
        "1",
        "12",
        "123",
        "1234",
        "12345",
        "123456",
        "1234567",
        "12345678",
        "123456789",
        "12345678901",
        "123456789012",
        "1234567890123",
        "12345678901234",
        "123456789012345",
        "1234567890123456",
        "12345678901234567" };
    for (size_t i = 0; i < H_ARRAYSIZE(s); ++i) {
        for (size_t j = 0; j < H_ARRAYSIZE(s); ++j) {
            auto name = s[j];
            auto conf = s[i];
            H_TEST_ASSERT(m.Add(name, conf, &Creator));
            H_TEST_ASSERT(m.Reload(name, conf));
            simcc::DoubleBuffering* tt = m.Get(name);
            H_TEST_ASSERT(tt);
            UTTarget* t = static_cast<UTTarget*>(tt->Get().get());
            H_TEST_ASSERT(t->conf() == conf);
            H_TEST_ASSERT(t->RefCount() == 1);

            UTTargetPTr ut = t;
            H_TEST_ASSERT(ut->RefCount() == 2);

            ut = NULL;
            H_TEST_ASSERT(t->RefCount() == 1);

            tt = NULL;
            H_TEST_ASSERT(t->RefCount() == 1);
        }
    }
}
