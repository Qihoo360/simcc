#include "wcpp/string_util.h"
#include "wcpp/data_stream.h"
#include "wcpp/utility.h"

#include "json.h"
#include "json_tokener.h"
#include "json_utf8_inl.h"

namespace wcpp {
namespace json {

JSONObject::JSONObject(const std::string& source)
    : Object(kJSONObject) {
    Parse(source);
}

JSONObject::JSONObject(const char* source, const wcpp::int32 source_len /*= -1*/)
    : Object(kJSONObject) {
    Parse(source, source_len);
}

JSONObject::JSONObject(JSONTokener* token)
    : Object(kJSONObject) {
    Parse(token);
}

JSONObject::~JSONObject() {
}

wcpp::uint32 JSONObject::Parse(JSONTokener* x) {
    if (!x->SkipComment()) {
        set_error(kCommentFormatError, x);
        return 0;
    }

    char c = x->NextClean();
    if (c != '{') {
        set_error(kJSONObjectNotBeginWithBraces, x);
        return 0;
    }

    std::string key;
    for (;;) {
        if (!x->SkipComment()) {
            set_error(kCommentFormatError, x);
            return 0;
        }

        c = x->NextClean();
        switch (c) {
        case 0:
            set_error(kJSONObjectNotEndWithBraces, x);
            return 0;
        case '}':
            return x->GetCurrentPosition();
        case '"':   // a key must be a string
            if (!x->NextString('"', key)) {
                set_error(kJSONObjectKeyNotString, x);
                return 0;
            } else if (key.size() == 0) {
                set_error(kJSONObjectKeyIsEmpty, x);
                return 0;
            }
            break;
        default:
            set_error(kInvalidCharacter, x);
            return 0;
        }

        // The key is followed by ':'
        if (!x->SkipComment()) {
            set_error(kCommentFormatError, x);
            return 0;
        }

        c = x->NextClean();

        if (c != ':') {
            set_error(kKeyValueSeperatorError, x);
            return 0;
        }

        Object* po = x->NextValue(this);

        if (po) {
            Put(key, po);
        } else {
            // error code has been set by nextValue
            return 0;
        }

        if (!x->SkipComment()) {
            set_error(kCommentFormatError, x);
            return 0;
        }

        c = x->NextClean();

        // pairs are separated by ','
        switch (c) {
        case ',':
            if (!x->SkipComment()) {
                set_error(kCommentFormatError, x);
                return 0;
            }

            c = x->NextClean();
            if (c == '}') {
                return x->GetCurrentPosition(); // ok, we have parsed done here
            }

            x->Back();
            break;
        case '}':
            return x->GetCurrentPosition();
        default:
            set_error(kInvalidCharacter, x);
            return 0;
        }
    }
}

wcpp::uint32 JSONObject::Parse(const char* source, const wcpp::int64 source_len) {
    if (source_len == 0 || !source) {
        set_error(kParameterWrong);
        return 0;
    }

    json::JSONTokener x(source, source_len);

    return Parse(&x);
}

wcpp::uint32 JSONObject::Parse(const std::string& source) {
    json::JSONTokener x(source);

    return Parse(&x);
}

bool JSONObject::Put(const std::string& key, Object* value) {
    if (!value) {
        map_.erase(key);
        return true;
    }

    map_[key] = value;
    return true;
}

bool JSONObject::Put(const std::string& key, const ObjectPtr& value) {
    return Put(key, value.get());
}

bool JSONObject::Put(const std::string& key, const wcpp::int64 value) {
    JSONInteger* jo = new JSONInteger(value);
    return Put(key, jo);
}

bool JSONObject::Put(const std::string& key, const wcpp::float64 value) {
    JSONDouble* jo = new JSONDouble(value);
    return Put(key, jo);
}

bool JSONObject::Put(const std::string& key, const bool value) {
    JSONBoolean* jo = new JSONBoolean(value);
    return Put(key, jo);
}

bool JSONObject::Put(const std::string& key, const std::string& value) {
    return Put(key, value.c_str());
}

bool JSONObject::Put(const std::string& key, const char* value) {
    JSONString* jo = new JSONString(value);
    return Put(key, jo);
}

namespace {
inline bool is_float_number(const char* s , size_t len) {
    for (size_t i = 0; i < len; ++i) {
        if (s[i] == '.' || s[i] == 'e' || s[i] == 'E') {
            return true;
        }
    }
    return false;
}
}

Object* JSONObject::ConvertToObject(const char* s, size_t len, JSONParser* parser, JSONTokener* x) {
    assert(len > 0);

    char b = s[0]; //beginning char
#if 1
    switch (b) {
    case 'n':
        if (strncmp(s + 1, "ull", 3) == 0) {
            return new JSONNull();
        }
        break;
    case 't':
        if (strncmp(s + 1, "rue", 3) == 0) {
            return new JSONBoolean(true);
        }
        break;
    case 'f':
        if (strncmp(s + 1, "alse", 4) == 0) {
            return new JSONBoolean(false);
        }
        break;
    default:
        break;
    }
#else
    if (b == 'n' && strncmp(s + 1, "ull", 3) == 0) {
        return new JSONNull();
    } else if (b == 't' && strncmp(s + 1, "rue", 3) == 0) {
        return new JSONBoolean(true);
    } else if (b == 'f' && strncmp(s + 1, "alse", 4) == 0) {
        return new JSONBoolean(false);
    }
#endif

    /*
     * If it might be a number, try converting it. We support the 0- and 0x-
     * conventions. If a number cannot be produced, then the value will just
     * be a string. Note that the 0-, 0x-, plus, and implied string
     * conventions are non-standard. A JSON parser is free to accept
     * non-JSON forms as long as it accepts all correct JSON forms.
     */
    if (b != '.' && b != '-' && b != '+' && (b < '0' || b > '9')) {
        if (parser) {
            parser->set_error(JSONParser::kInvalidIntegerOrDoubleString, x);
        }
        return NULL;
    }

    /* a normal number string */
    if (b != '0') {
        if (is_float_number(s, len)) {
            return new JSONDouble(std::atof(s));
        } else {
            return new JSONInteger(std::atoll(s));
        }
    }

    /* hexadecimal number */
    if (len > 2 && (s[1] == 'x' || s[1] == 'X')) {
        wcpp::int64 result = 0;
        int curval = 0;

        for (size_t i = 2; i < len; i++) {
            curval = JSONTokener::DehexChar(s[i]);

            if (curval != -1) {
                result = (result << 4) + curval;
            } else {
                if (parser) {
                    parser->set_error(JSONParser::kInvalidHexadecimalCharacter, x);
                }
                return NULL;
            }
        }

        return new JSONInteger(result);
    } else {
        if (is_float_number(s, len)) {
            return new JSONDouble(std::atof(s));
        } else {
            /* it is a octal number string */
            wcpp::int64 result = 0;
            int curval = 0;

            for (size_t i = 1; i < len; i++) {
                curval = JSONTokener::DehexChar(s[i]);

                if (curval != -1) {
                    result = (result << 3) + curval;
                } else {
                    if (parser) {
                        parser->set_error(JSONParser::kInvalidHexadecimalCharacter, x);
                    }
                    return NULL;
                }
            }

            return new JSONInteger(result);
        }
    }
}

void JSONObject::Quote(const std::string& source, bool utf8_to_unicode, wcpp::DataStream& sb) {
    sb.Expand(source.size() << 2);

    sb.Write('"');

    const char* readp = source.data();
    const char* readend = readp + source.size();
    register char c = 0;
    while (readp < readend) {
        c = *readp++;
        switch (c) {
        case '\\':
            sb.Write("\\\\", 2);
            break;
        case '"':
            sb.Write("\\\"", 2);
            break;
        case '/':
            // Even though \/ is considered a legal escape in JSON, a bare
            // slash is also legal, so I see no reason to escape it.
            // (I hope I am not misunderstanding something.)
            // notes: actually escaping \/ may be useful in javascript to avoid </ sequence.
            // So I add a flag to allow this compatibility mode and prevent this
            // sequence from occurring.
#if 1
            if (readp > source.data() && *(readp - 1) == '<') {
                sb.Write('\\');
            }
            sb.Write(c);
#else
            sb.Write("\\/", 2);
#endif
            break;
        case '\b':
            sb.Write("\\b", 2);
            break;
        case '\t':
            sb.Write("\\t", 2);
            break;
        case '\n':
            sb.Write("\\n", 2);
            break;
        case '\r':
            sb.Write("\\r", 2);
            break;
        case '\f':
            sb.Write("\\f", 2);
            break;
        default:
            if (utf8_to_unicode) {
                //Reference of jansson-2.0.1 http://www.digip.org/jansson/
                int32_t codepoint = 0;
                const char* end = utf8_iterate(readp - 1, &codepoint);
                if (!end) {
                    //This is not a ASCII code and also NOT an UTF8 code,
                    //Maybe it is GBK Chinese code, so we just write it
                    sb.Write(c);
                    break;
                }

                if (codepoint <= 0x7F) {
                    sb.Write(static_cast<char>(codepoint));
                } else {
                    JSONTokener::EncodeUnicodeNumber(codepoint, sb);
                    readp = end;
                }
                break;
            } else {
                sb.Write(c);
            }
            break;
        }
    }

    sb.Write('"');
}

Object* JSONObject::Get(const std::string& key)const {
    const_iterator it = map_.find(key);
    if (it != map_.end()) {
        return (it->second);
    }

    return NULL;
}

JSONBoolean* JSONObject::GetJSONBoolean(const std::string& key) const {
    const_iterator it = map_.find(key);
    if (it != map_.end()) {
        return cast<JSONBoolean>(it->second.get());
    }

    return NULL;
}

JSONDouble* JSONObject::GetJSONDouble(const std::string& key)const {
    const_iterator it = map_.find(key);
    if (it != map_.end()) {
        return cast<JSONDouble>((it->second).get());
    }

    return NULL;
}

JSONInteger* JSONObject::GetJSONInteger(const std::string& key)const {
    const_iterator it = map_.find(key);
    if (it != map_.end()) {
        return cast<JSONInteger>((it->second).get());
    }

    return NULL;
}

JSONArray* JSONObject::GetJSONArray(const std::string& key)const {
    const_iterator it = map_.find(key);
    if (it != map_.end()) {
        return cast<JSONArray>((it->second).get());
    }

    return NULL;
}

JSONObject* JSONObject::GetJSONObject(const std::string& key)const {
    const_iterator it = map_.find(key);
    if (it != map_.end()) {
        return cast<JSONObject>((it->second).get());
    }

    return NULL;
}

JSONString* JSONObject::GetJSONString(const std::string& key)const {
    const_iterator it = map_.find(key);
    if (it != map_.end()) {
        return cast<JSONString>((it->second).get());
    }

    return NULL;
}



void JSONObject::ToString(std::string& s, bool readable, bool utf8_to_unicode) const {
    size_t indent = 0;
    if (readable) {
        indent = 1;
    }

    buf_.Reset();
    ToStringBuf(buf_, indent, utf8_to_unicode);
    s.assign(buf_.data(), buf_.size());
}

void JSONObject::ToStringBuf(wcpp::DataStream& sb, size_t indent, bool utf8_to_unicode) const {
    // write begin
    static const char* indent_str = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";
    if (indent > 1) {
        sb.Write(indent_str,  indent - 1);
    }

    sb.Write('{');

    if (indent > 0) {
        sb.Write('\n');
    }

    ConstIterator it(map_.begin()), ite(map_.end());
    bool need_comma = false;

    for (; it != ite; it++) {
        if (need_comma) {
            sb.Write(',');

            if (indent > 0) {
                sb.Write('\n');
            }
        } else {
            need_comma = true;
        }

        // write key
        if (indent > 0) {
            sb.Write(indent_str, indent);
            JSONObject::Quote(it->first, utf8_to_unicode, sb);
        } else {
            JSONObject::Quote(it->first, utf8_to_unicode, sb);
        }

        // write value
        sb.Write(':');

        if (indent > 0) {
            switch (it->second->type()) {
            case kJSONObject:
            case kJSONArray:
                sb.Write('\n');
                it->second->ToStringBuf(sb, indent + 1, utf8_to_unicode);
                break;
            default:
                it->second->ToStringBuf(sb, 0, utf8_to_unicode);
            }

        } else {
            it->second->ToStringBuf(sb, 0, utf8_to_unicode);
        }
    }

    // write end close }
    if (indent > 0) {
        sb.Write('\n');
        sb.Write(indent_str, indent - 1);
    }

    sb.Write('}');
}



bool JSONObject::GetBool(const std::string& strKey, bool default_value)const {
    JSONBoolean* pVal = GetJSONBoolean(strKey);

    if (pVal) {
        return pVal->value();
    }

    return default_value;
}


wcpp::float64 JSONObject::GetDouble(const std::string& strKey, wcpp::float64 default_value)const {
    JSONDouble* pVal =  GetJSONDouble(strKey);

    if (pVal) {
        return pVal->value();
    }

    return default_value;
}


wcpp::float64 JSONObject::GetDecimal(const std::string& strKey, wcpp::float64 default_value)const {
    Object* o = Get(strKey);
    if (o) {
        if (o->IsTypeOf(kJSONDouble)) {
            return ((JSONDouble*)o)->value();
        }

        if (o->IsTypeOf(kJSONInteger)) {
            return ((JSONInteger*)o)->value();
        }
    }
    return default_value;
}

wcpp::int64 JSONObject::GetInteger(const std::string& strKey, wcpp::int64 default_value)const {
    JSONInteger* v = GetJSONInteger(strKey);
    if (v) {
        return v->value();
    }

    return default_value;
}


const std::string&  JSONObject::GetString(const std::string& strKey, const std::string& default_value)const {
    JSONString* v =  GetJSONString(strKey);
    if (v) {
        return v->value();
    }

    return default_value;
}

template<class T>
bool JSONObject::PutIntegerArray(const std::string& key, const T* value, wcpp::uint32 count) {
    JSONArray* array = new JSONArray();
    for (wcpp::uint32 i = 0; i < count; ++i) {
        array->Put(new JSONInteger(static_cast<wcpp::int64>(value[i])));
    }
    return Put(key, array);
}

bool JSONObject::PutByteArray(const std::string& key, const wcpp::int8* value, wcpp::uint32 count) {
    return PutIntegerArray<wcpp::int8>(key, value, count);
}

bool JSONObject::PutInt32Array(const std::string& key, const wcpp::int32* value, wcpp::uint32 count) {
    return PutIntegerArray<wcpp::int32>(key, value, count);
}

bool JSONObject::PutInt64Array(const std::string& key, const wcpp::int64* value, wcpp::uint32 count) {
    return PutIntegerArray<wcpp::int64>(key, value, count);
}

bool JSONObject::PutFloat32Array(const std::string& key, const wcpp::float32* value, wcpp::uint32 count) {
    JSONArray* array = new JSONArray();
    for (wcpp::uint32 i = 0; i < count; ++i) {
        array->Put(new JSONDouble(wcpp::float64(value[i])));
    }

    return Put(key, array);
}

bool JSONObject::PutFloat64Array(const std::string& key, const wcpp::float64* value, wcpp::uint32 count) {
    JSONArray* array = new JSONArray();
    for (wcpp::uint32 i = 0; i < count; ++i) {
        array->Put(new JSONDouble(wcpp::float64(value[i])));
    }
    return Put(key, array);
}


bool JSONObject::PutBoolArray(const std::string& key, const bool* value, wcpp::uint32 count) {
    JSONArray* array = new JSONArray();
    for (wcpp::uint32 i = 0; i < count; ++i) {
        array->Put(new JSONBoolean(value[i]));
    }
    return Put(key, array);
}


bool JSONObject::PutStringArray(const std::string& key, const std::string* value, wcpp::uint32 count) {
    JSONArray* array = new JSONArray();
    for (wcpp::uint32 i = 0; i < count; ++i) {
        array->Put(new JSONString(value[i]));
    }
    return Put(key, array);
}

void JSONObject::Merge(const JSONObject* rhs, bool override) {
    if (!rhs) {
        return;
    }

    const_iterator itrhs(rhs->map_.begin()), iterhs(rhs->map_.end());
    for (; itrhs != iterhs; ++itrhs) {
        iterator iterthis = map_.find(itrhs->first);
        if (iterthis == map_.end()) {
            map_[itrhs->first] = itrhs->second;
            continue;
        }

        // recursive Merge
        json::ObjectPtr& joriginal = map_[itrhs->first];
        if (joriginal->IsTypeOf(kJSONObject) && itrhs->second->IsTypeOf(kJSONObject)) {
            static_cast<json::JSONObject*>(joriginal.get())->Merge(static_cast<json::JSONObject*>(itrhs->second.get()), override);
            continue;
        }

        if (override) {
            joriginal = itrhs->second;
        }
    }
}

bool JSONObject::LoadFrom(wcpp::DataStream& file) {
    uint32 sz = 0;
    file >> sz;
    std::string key;
    for (uint32 i = 0; i < sz; i++) {
        file >> key;
        Object* o = NULL;
        Object::DeserializeOneObject(file, o);
        if (o) {
            Put(key, o);
            o = NULL;
        } else {
            set_error(kDeserializeBinaryDataError);
            return false;
        }
    }

    return true;
}

void JSONObject::SaveTo(wcpp::DataStream& file) const {
    file << (wcpp::uint8)type()
         << (wcpp::uint32)map_.size();
    auto it(map_.begin()), ite(map_.end());
    for (; it != ite; it++) {
        file << it->first;
        it->second->SaveTo(file);
    }
}


bool JSONObject::Equals(const Object& rhs) {
    if (rhs.type() != type()) {
        return false;
    }

    const JSONObject& rhsJSONOBject = static_cast<const JSONObject&>(rhs);
    if (rhsJSONOBject.map_.size() != map_.size()) {
        return false;
    }

    auto itthis(map_.begin());
    auto itethis(map_.end());
    auto itrhs(rhsJSONOBject.begin());
    auto iterhs(rhsJSONOBject.end());
    for (; itthis != itethis && itrhs != iterhs; ++itthis, ++itrhs) {
        if (itthis->first != itrhs->first) {
            return false;
        }

        if (!itthis->second->Equals(*(itrhs->second))) {
            return false;
        }
    }

    return true;
}

void JSONObject::Remove(const Object* pobj) {
    iterator it(map_.begin());
    iterator ite(map_.end());
    for (; it != ite; ++it) {
        if (it->second == pobj) {
            map_.erase(it);
        }
    }

}

// Save, Serializer. Save the object into a memory data stream
wcpp::DataStream& operator << (wcpp::DataStream& file, const JSONObject& val) {
    val.SaveTo(file);
    return file;
}

// Load, Deserialize. Load data from memory data stream and convert it to an object
wcpp::DataStream& operator >> (wcpp::DataStream& file, JSONObject& val) {
    wcpp::uint8 type = 0xff;
    file >> type;
    assert(type == kJSONObject);
    if (!val.LoadFrom(file)) {
        val.set_error(JSONObject::kLoadBinaryDataError);
    }
    return file;
}



#ifdef H_PROVIDE_PROTOBUF_DEBUGSTRING_2_JSON
namespace {
bool parseObject(JSONObject* rootjson, JSONTokener* x);
Object* parseValue(JSONTokener* x);
std::string parseKey(JSONTokener* x);

Object* parseValue(JSONTokener* x) {
    char c = x->NextClean();
    switch (c) {
    case '"':
    case '\'': {
        JSONString* jstring = new JSONString();
        if (!x->NextString(c, true, jstring->value())) {
            delete jstring;
            jstring = NULL;
        }
        return jstring;
    }
    default:
        //Handle unquoted text like: true, false, or null, or it can be a number.
        break;
    }

    /*
    * Handle unquoted text. This could be the values true, false, or
    * null, or it can be a number. An implementation (such as this one)
    * is allowed to also accept non-standard forms.
    *
    * Accumulate characters until we reach the end of the text or a
    * formatting character.
    */

    //the static table of ",:]}/\\\"[{;=#" and space, tab and control characters
    static char specialchars[256] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };

    const char* startpos = x->GetCurrent() - 1;
    while (specialchars[(int)c]) {
        c = x->Next();
    }

    x->Back();

    if (x->GetCurrent() <= startpos) {
        printf("Miss value\n");
        return NULL;
    }

    Object* value = JSONObject::ConvertToObject(startpos, x->GetCurrent() - startpos, NULL, x);
    if (!value) {
        JSONString* js = new JSONString;
        value = js;
        std::string strv(startpos, x->GetCurrent() - startpos);
        js->value() = strv;
    }
    return value;
}

std::string parseKey(JSONTokener* x) {
    x->SkipSpaces();
    const char* start = x->GetCurrent();
    for (;;) {
        char c = x->Next();
        if (c == ':' || c == '{') {
            break;
        } else if (x->IsEnd()) {
            return "";
        }
    }

    x->Back();
    std::string key(start, x->GetCurrent() - start);
    wcpp::StringUtil::Trim(key);
    return key;
}

void putObject(const std::string& key, JSONObject* rootjson, Object* value) {
    JSONArrayPtr original_arr = rootjson->GetJSONArray(key);
    if (original_arr) {
        original_arr->Put(value);
    } else {
        ObjectPtr original_obj = rootjson->Get(key);
        if (original_obj) {
            JSONArrayPtr ja = new JSONArray;
            ja->Put(original_obj);
            ja->Put(value);
            rootjson->Put(key, ja.get());
        } else {
            rootjson->Put(key, value);
        }
    }
}

bool parseObject(JSONObject* rootjson, JSONTokener* x) {
    x->SkipSpaces();
    while (!x->IsEnd()) {
        char c = x->NextClean();
        if (c == '}' || c == 0) {
            return true;
        }
        x->Back();
        std::string key = parseKey(x);
        if (key.empty()) {
            return true;
        }
        c = x->NextClean();
        switch (c) {
        case ':': {
            putObject(key, rootjson, parseValue(x));
        }
        break;
        case '{': {
            JSONObjectPtr value = new JSONObject;
            if (parseObject(value.get(), x)) {
                putObject(key, rootjson, value);
            } else {
                printf("Invalid object, error location:%u %s", x->GetCurrentPosition(), x->GetCurrent());
                return false;
            }
        }
        break;
        default:
            printf("Invalid char [%c], error location:%u %s", c, x->GetCurrentPosition(), x->GetCurrent());
            return false;
            break;
        }
    }

    return true;
}
}//end of namespace

wcpp::uint32 JSONObject::ParseProtobuf(const std::string& protobuf_debug_string) {
    JSONTokener x(protobuf_debug_string.data(), protobuf_debug_string.size());
    parseObject(const_cast<JSONObject*>(this), &x);
    return x.GetCurrentPosition();
}
#endif

}
}
