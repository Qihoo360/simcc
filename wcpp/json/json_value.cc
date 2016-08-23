#include "wcpp/string_util.h"
#include "wcpp/data_stream.h"
#include "wcpp/utility.h"
#include "json.h"
#include "json_tokener.h"

namespace wcpp {
namespace json {

JSONInteger::JSONInteger(wcpp::int64 v)
    : BaseClass(v) {
}

JSONInteger& JSONInteger::operator=(wcpp::int64 v) {
    value_ = v;
    return *this;
}

namespace {
inline void WriteUint64(wcpp::uint64 ui64, wcpp::DataStream& ds) {
    char buffer[20];
    char* p = buffer;
    do {
        *p++ = char(ui64 % 10) + '0';
        ui64 /= 10;
    } while (ui64 > 0);

    do {
        --p;
        ds.Write(*p);
    } while (p != buffer);
}

inline void WriteInt64(wcpp::int64 i64, wcpp::DataStream& ds) {
    if (i64 < 0) {
        ds.Write('-');
        i64 = -i64;
    }
    WriteUint64((uint64_t)i64, ds);
}
}

void JSONInteger::ToString(std::string& s, bool /*readable*/, bool /*utf8_to_unicode*/)const {
    s = std::to_string(value_);
}

void JSONInteger::ToStringBuf(wcpp::DataStream& sb, size_t indent, bool /*utf8_to_unicode*/)const {
    for (size_t i = 1; i < indent; i++) {
        sb.Write('\t');
    }

#if 1
    WriteInt64(value_, sb);
#else
    std::string s;
    ToString(s);
    sb.Write(s.c_str(), s.length());
#endif
}

bool JSONInteger::Equals(const Object& rhs) {
    if (rhs.type() == type() && dynamic_cast<const JSONInteger&>(rhs).value_ == value_) {
        return true;
    }

    return false;
}

bool JSONInteger::LoadFrom(wcpp::DataStream& file) {
    file >> value_;
    return true;
}

void JSONInteger::SaveTo(wcpp::DataStream& file) const {
    file << (wcpp::uint8)type()
         << value_;
}

//------------------------------------------------------------------

JSONDouble::JSONDouble(wcpp::float64 v)
    : BaseClass(v) {}

JSONDouble& JSONDouble::operator=(wcpp::float64 v) {
    value_ = v;
    return *this;
}

bool JSONDouble::Equals(const JSONDouble& other, wcpp::float64 tolarence) {
    return wcpp::Util::Equals(value_, other.value_, tolarence);
}

bool JSONDouble::Equals(wcpp::float64 v, wcpp::float64 tolarence) {
    return wcpp::Util::Equals(value_, v, tolarence);
}

void JSONDouble::ToString(std::string& s, bool /*readable*/, bool /*utf8_to_unicode*/)const {
    s = std::to_string(value_);
    if (s.find('.') != std::string::npos) {
        wcpp::StringUtil::Trim(s, '0', false);
        if (*s.rbegin() == '.') {
            s.append(1, '0');
        }
    } else {
        //TODO fix JSONDouble to int   
    }
}

namespace {
    // TODO Optimization with custom double-to-string converter.
    inline void WriteDouble(double d, wcpp::DataStream& ds) {
        enum { kDoubleMaxLen = 64 };
        ds.Expand(kDoubleMaxLen);
        int ret = snprintf(reinterpret_cast<char*>(ds.GetCurrentWriteBuffer()), kDoubleMaxLen - 1, "%f", d);
        assert(ret >= 1);
        const char* p = reinterpret_cast<const char*>(ds.GetCurrentWriteBuffer());
        ds.seekp(ret);
        const char* pend = reinterpret_cast<const char*>(ds.GetCurrentWriteBuffer());
        assert(pend - p == ret);
        while (p != pend) {
            if (*p++ == '.') {
                break;
            }
        }

        if (p == pend) {
            ds.Write(".0", 2); // make sure the output is a double number
        }
    }
}

void JSONDouble::ToStringBuf(wcpp::DataStream& sb, size_t indent, bool /*utf8_to_unicode*/)const {
    for (size_t i = 1; i < indent; i++) {
        sb.Write('\t');
    }
#if 1
    WriteDouble(value_, sb);
#else
    std::string s;
    ToString(s);
    sb.Write(s.c_str(), s.length());
#endif
}

bool JSONDouble::Equals(const Object& rhs) {
    if (rhs.type() == type() && wcpp::Util::Equals(dynamic_cast<const JSONDouble&>(rhs).value_, value_)) {
        return true;
    }

    return false;
}

void JSONDouble::SaveTo(wcpp::DataStream& file) const {
    file << (wcpp::uint8)type()
        << value_;
}

bool JSONDouble::LoadFrom(wcpp::DataStream& file) {
    file >> value_;
    return true;
}


//------------------------------------------------------------------

JSONBoolean::JSONBoolean(bool v)
    : BaseClass(v) {}

JSONBoolean& JSONBoolean::operator=(bool v) {
    value_ = v;
    return *this;
}


void JSONBoolean::ToString(std::string& s, bool /*v*/, bool /*utf8_to_unicode*/)const {
    static const std::string t = "true";
    static const std::string f = "false";
    s = value_ ? t : f;
}

void JSONBoolean::ToStringBuf(wcpp::DataStream& sb, size_t indent, bool /*utf8_to_unicode*/)const {
    for (size_t i = 1; i < indent; i++) {
        sb.Write('\t');
    }

    enum { kTrueLen = 4, kFlaseLen = 5 };
    if (value_) {
        sb.Write("true", kTrueLen);
    } else {
        sb.Write("false", kFlaseLen);
    }
}

bool JSONBoolean::Equals(const Object& rhs) {
    if (rhs.type() == type() && dynamic_cast<const JSONBoolean&>(rhs).value_ == value_) {
        return true;
    }

    return false;
}

void JSONBoolean::SaveTo(wcpp::DataStream& file) const {
    file << (wcpp::uint8)type()
        << value_;
}
bool JSONBoolean::LoadFrom(wcpp::DataStream& file) {
    file >> value_;
    return true;
}


//------------------------------------------------------------------


JSONString::JSONString(const std::string& v)
    : BaseClass(v) {}

JSONString::JSONString(const char* v)
    : BaseClass(v) {}

JSONString& JSONString::operator=(const std::string& v) {
    value_ = v;
    return *this;
}

void JSONString::ToString(std::string& s, bool /*readable*/, bool utf8_to_unicode)const {
    wcpp::DataStream buf(512);
    JSONObject::Quote(value_, utf8_to_unicode, buf);
    s.assign(buf.data(), buf.size());
}

void JSONString::ToStringBuf(wcpp::DataStream& sb, size_t indent, bool utf8_to_unicode)const {
    for (size_t i = 1; i < indent; i++) {
        sb.Write('\t');
    }

    JSONObject::Quote(value_, utf8_to_unicode, sb);
}

bool JSONString::Equals(const Object& rhs) {
    if (rhs.type() == type() && dynamic_cast<const JSONString&>(rhs).value_ == value_) {
        return true;
    }

    return false;
}

void JSONString::SaveTo(wcpp::DataStream& file) const {
    file << (wcpp::uint8)type()
        << value_;
}

bool JSONString::LoadFrom(wcpp::DataStream& file) {
    file >> value_;
    return true;
}

//------------------------------------------------------------------


void JSONNull::ToString(std::string& s, bool /*readable*/, bool /*utf8_to_unicode*/)const {
    static const std::string n = "null";
    s = n;
}

void JSONNull::ToStringBuf(wcpp::DataStream& sb, size_t indent, bool /*utf8_to_unicode*/)const {
    for (size_t i = 1; i < indent; i++) {
        sb.Write('\t');
    }

    sb.Write("null", 4);
}

bool JSONNull::Equals(const Object& rhs) {
    if (rhs.type() == type()) {
        return true;
    }

    return false;
}

void JSONNull::SaveTo(wcpp::DataStream& file) const {
    file << (wcpp::uint8)type();
}

bool JSONNull::LoadFrom(wcpp::DataStream& /*file*/) {
    return true;
}


}
}