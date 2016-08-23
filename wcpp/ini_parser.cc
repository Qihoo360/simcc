
#include "wcpp/ini_parser.h"
#include "wcpp/data_stream.h"
#include "wcpp/tokener.h"
#include "wcpp/string_util.h"

namespace wcpp {

INIParser::INIParser(bool _case_sensitive /*= true*/, bool _compatible /*= false*/, bool keep_sequence /*= false*/)
    : case_sensitive_(_case_sensitive)
    , stop_parsing_(false)
    , keep_sequence_(keep_sequence)
    , error_code_(kNoError)
    , compatible_(_compatible)
    , trim_chars_(" \t\r\n") {
    memset(trim_chars_table_, 0, sizeof(trim_chars_table_));
    InitTrimCharsTable();
}

INIParser::~INIParser() {
    listeners_.clear();
}

bool INIParser::Parse(const string& filename) {
    return Parse(filename.c_str());
}

bool INIParser::Parse(const char* filename) {
    DataStream ds;
    if (!ds.ReadFile(filename)) {
        return false;
    }

    return Parse((const char*)ds.GetCache(), ds.size(), "\n", "=");
}

bool INIParser::Parse(const char* data, size_t datalen) {
    return Parse(data, datalen, "\n", "=");
}

class INIParser::SequenceParseListener {
public:
    SequenceParseListener(INIParser* parser) : parser_(parser) {
        listener_ = std::bind(&SequenceParseListener::OnValue, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        parser_->SetParseListener(listener_);
    }

    ~SequenceParseListener() {
    }

    virtual void OnValue(::wcpp::INIParser& /*parser*/, const std::string& section, const std::string& key, const std::string& /*value*/) {
        INIParser::StringList* keys = NULL;
        auto it = parser_->section_list_.rbegin();
        if (it != parser_->section_list_.rend() && it->first == section) {
            keys = &(it->second);
        } else {
#if _MSC_VER >= 1800
            parser_->section_list_.push_back(std::make_pair<string, INIParser::StringList>((string)section, INIParser::StringList()));
#else
#if H_COMPILER_VERSION >= 472
            parser_->section_list_.push_back(std::make_pair<string, INIParser::StringList>((string)section, INIParser::StringList()));
#else
            parser_->section_list_.push_back(std::make_pair<string, INIParser::StringList>(section, INIParser::StringList()));
#endif
#endif
            keys = &(parser_->section_list_.rbegin()->second);
        }
        keys->push_back(key);
    }

private:
    INIParser* parser_;
    INIParser::ParseListener listener_;
};

bool INIParser::Parse(const char* data, size_t datalen, const char* linesep, const char* kvsep) {
    return Parse(data, datalen, string(linesep), string(kvsep));
}

bool INIParser::Parse(const char* data, size_t data_len, const string& line_sep, const string& kv_sep) {
    if (!data || 0 == data_len || line_sep.empty() || kv_sep.empty()) {
        return false;
    }

    // avoid string ending with many '\0'
    while (data_len > 0 && data[data_len - 1] == 0) {
        data_len -= 1;
    }

    if (data_len == 0) {
        return true;
    }

    std::unique_ptr<SequenceParseListener> listener;
    if (keep_sequence_) {
        listener.reset(new SequenceParseListener(this));
    }

    const char* data_end = data + data_len;

    kv_sep_ = kv_sep;
    line_sep_ = line_sep;

    const char* line_begin = data;
    const char* line_end = NULL;
    size_t line_sep_len = line_sep.size();
    size_t kv_sep_len   = kv_sep.size();
    string line;
    string section;
    const char kSectionOpen = '[';
    const char kSectionClose = ']';
    while (line_begin && !stop_parsing_ && line_begin < data_end) {
        if (!trim_chars_.empty()) {
            line_begin = SkipTrimChars(line_begin, data_end);
            line_begin = SkipCommits(line_begin, data_end - line_begin);
            line_begin = SkipTrimChars(line_begin, data_end);

            if (!line_begin || line_begin >= data_end) {
                // parse finished
                break;
            }
        }


        if (strncmp(line_begin, line_sep.data(), line_sep_len) == 0) {
            // skip one empty
            // fix bug:
            //  "a:1||||c:3"
            //  ini.Parse(d.data(), d.size(), "||", ":");
            line_begin += line_sep_len;
            continue;
        }


        if (*line_begin == kSectionOpen) {
            line_end = strstr(line_begin, line_sep.data());
            if (!line_end) {
                // last line is : "["
                if (line_begin + 1 == data_end) {
                    error_code_ = kErrorSectionFormatWrong;
                    if (compatible_) {
                        break;
                    } else {
                        return false;
                    }
                }

                // last line is : "[ "
                assert(line_begin + 1 < data_end);
                section = string(line_begin + 1, data_end - line_begin - 1);
                if (!trim_chars_.empty()) {
                    StringUtil::Trim(section, trim_chars_.c_str(), true, true);
                }
                if (section.empty()) {
                    return true;
                }

                if (!case_sensitive_) {
                    std::transform(section.begin(), section.end(), section.begin(), towlower);
                }

                section_map_[section] = ssmap();
                return true;
            }

            const char* section_end = strchr(line_begin, kSectionClose);
            if (!section_end || section_end <= line_begin || section_end >= line_end) {
                error_code_ = kErrorSectionFormatWrong;
                if (compatible_) {
                    // try to read next line
                    section = string(line_begin + 1, line_end - line_begin - 1);
                    if (!trim_chars_.empty()) {
                        StringUtil::Trim(section, trim_chars_.c_str(), true, true);
                    }
                    line_begin = line_end + line_sep_len;
                    continue;
                } else {
                    return false;
                }
            }

            section = string(line_begin + 1, section_end - line_begin - 1);
            if (!trim_chars_.empty()) {
                StringUtil::Trim(section, trim_chars_.c_str(), true, true);
            }
            if (!case_sensitive_) {
                std::transform(section.begin(), section.end(), section.begin(), towlower);
            }

            line_begin = line_end + line_sep_len;
            continue;
        }

        const char* kv_sep_pos = strstr(line_begin, kv_sep.data());
        line_end  = strstr(line_begin, line_sep.data());
        Status e = CheckStatus(data_end, kv_sep_pos, line_end);
        switch (e) {
        case kFoundKeyAndValue:
            break;
        case kFinishedError:
            return false;
        case kNeedSkipThisLine:
            line_begin = line_end + line_sep_len;
            continue;// for-loop continue
            break;
        case kFinishedOK:
            return true;
        default:
            assert(false);
            break;
        }

        string key   = string(line_begin, kv_sep_pos - line_begin);
        string value;
        if (line_end) {
            value = string(kv_sep_pos + kv_sep_len, line_end - kv_sep_pos - kv_sep_len);
        } else {
            value = string(kv_sep_pos + kv_sep_len, data_end - kv_sep_pos - kv_sep_len);
        }

        if (!trim_chars_.empty()) {
            StringUtil::Trim(key, trim_chars_.c_str(), true, true);
            StringUtil::Trim(value, trim_chars_.c_str(), true, true);
        }
        if (key.length()) {
            if (!case_sensitive_) {
                std::transform(key.begin(), key.end(), key.begin(), towlower);
            }

            section_map_[section][key] = value;
            if (!listeners_.empty()) {
                for (auto it(listeners_.begin()), ite(listeners_.end()); it != ite; ++it) {
                    (*it)(*this, section, key, value);
                }
            }
        }

        if (!line_end) {
            // parse finished
            break;
        }

        line_begin = line_end + line_sep_len;
    }

    return true;
}

void INIParser::SetParseListener(ParseListener pl) {
    listeners_.push_back(pl);
}

int64_t INIParser::GetInteger(const string& key, int64_t default_value /*= 0*/) const {
    return GetInteger(empty_string_, key, default_value);
}

int64_t INIParser::GetInteger(const string& section, const string& key, int64_t default_value /*= 0*/) const {
    bool found = false;
    const string& value = Get(section, key, &found);
    if (!found) {
        return default_value;
    }

    return std::stoll(value);
}

bool INIParser::GetBool(const string& key, bool default_value /*= false*/) const {
    return GetBool(empty_string_, key, default_value);
}

bool INIParser::GetBool(const string& section, const string& key, bool default_value /*= false*/) const {
    bool found = false;
    const string& value = Get(section, key, &found);
    if (!found) {
        return default_value;
    }

    return strnicmp(value.c_str(), "true", 4) == 0 ? true : false;
}

double INIParser::GetDouble(const string& key, double default_value /*= 0.0*/) const {
    return GetDouble(empty_string_, key, default_value);
}

double INIParser::GetDouble(const string& section, const string& key, double default_value /*= 0.0*/) const {
    bool found = false;
    const string& value = Get(section, key, &found);
    if (!found) {
        return default_value;
    }

    return std::stod(value);
}

const string& INIParser::Get(const string& key, bool* found) const {
    return Get(empty_string_, key, found);
}

const string& INIParser::Get(const string& section, const string& key, bool* found) const {
    string section_lower;
    string key_lower;

    const string* pkey = &key;
    const string* psection = &section;
    if (!case_sensitive_) {
        section_lower = section;
        key_lower = key;
        std::transform(section_lower.begin(), section_lower.end(), section_lower.begin(), towlower);
        std::transform(key_lower.begin(), key_lower.end(), key_lower.begin(), towlower);
        pkey = &key_lower;
        psection = &section_lower;
    }

    auto sectionit = section_map_.find(*psection);
    if (sectionit == section_map_.end()) {
        if (found) {
            *found = false;
        }
        return empty_string_;
    }

    auto it = sectionit->second.find(*pkey);

    if (it == sectionit->second.end()) {
        if (found) {
            *found = false;
        }
        return empty_string_;
    }

    if (found) {
        *found = true;
    }
    return it->second;
}

void INIParser::Set(const string& section, const string& key, const string& value) {
    section_map_[section][key] = value;
}

void INIParser::Set(const string& key, const string& value) {
    string section = "";
    Set(section, key, value);
}

const char* INIParser::SkipCommits(const char* szsrc, size_t len) {
    bool commit = false;
    Tokener token(szsrc, len);

    char c = token.NextClean();
    if (c == ';') {
        commit = true;
    } else if (c == '#') {
        commit = true;
    } else if (c == '/' && token.NextClean() == '/') {
        commit = true;
    }

    if (commit) {
        // If next line is also a committed string,
        // we need to continue skip it.
        token.SkipToNextLine();
        if (!token.IsEnd()) {
            return SkipCommits(token.GetCurrent(), token.GetReadableSize());
        }
    }

    return szsrc;
}

const char* INIParser::SkipSpaces(const char* szsrc) {
    const char* t = szsrc;
    for (; t && *t != '\0' && *t >= 0 && *t <= ' '; ++t) {
        ;//Do nothing
    }
    return t;
}

const INIParser::ssmap& INIParser::GetDefaultKeyValueMap() {
    return GetKeyValueMap(empty_string_);
}

const INIParser::ssmap& INIParser::GetKeyValueMap(const string& section) const {
    SectionMap::const_iterator iter(section_map_.find(section));
    if (iter != section_map_.end()) {
        return iter->second;
    }

    static INIParser::ssmap default_map;
    return default_map;
}

string INIParser::Serialize(bool input_order /*= false*/) const {
    string s;
    Serialize(s, input_order);
    return s;
}

void INIParser::Serialize(string& output, bool input_order /*= false*/) const {
    DataStream stream;
    Serialize(stream, input_order);
    output = string(stream.data(), stream.size());
}

void INIParser::Serialize(DataStream& stream, bool input_order /*= false*/) const {
    stream.Reserve(4096);
    serialize<DataStream>(stream, input_order);
}

void INIParser::Serialize(std::ostream& stream, bool input_order /*= false*/) const {
    serialize<std::ostream>(stream, input_order);
}

template< typename stream_t >
class SerializeVistor {
public:
    SerializeVistor() : stream_(NULL) {}

protected:
    void Visit(const INIParser& parser, const std::string& section, const std::string& key, const std::string& value) {
        assert(stream_);
        if (last_section_ != section) {
            last_section_ = section;
            stream_->write("[", 1);
            stream_->write(section.c_str(), section.length());
            stream_->write("]", 1);
            stream_->write(parser.line_separator().c_str(), parser.line_separator().size());
        }

        stream_->write(key.c_str(), key.length());
        stream_->write(parser.kv_separator().c_str(), parser.kv_separator().size());
        stream_->write(value.c_str(), value.length());
        stream_->write(parser.line_separator().c_str(), parser.line_separator().size());
    }

protected:
    void set_stream(stream_t* stream) {
        stream_ = stream;
    }

protected:
    stream_t* stream_;
    std::string last_section_;
};

template< typename stream_t >
class SerializeFastVistor : public SerializeVistor<stream_t> {
public:
    SerializeFastVistor(stream_t* stream) {
        SerializeVistor<stream_t>::set_stream(stream);
    }

    virtual void visit(const INIParser& parser, const std::string& section, const std::string& key, const std::string& value) {
        SerializeVistor<stream_t>::Visit(parser, section, key, value);
    }
};

template< typename stream_t >
class SerializeSequenceVistor : public SerializeVistor<stream_t> {
public:
    SerializeSequenceVistor(stream_t* stream) {
        SerializeVistor<stream_t>::set_stream(stream);
    }

    virtual void visit(const INIParser& parser, const std::string& section, const std::string& key, const std::string& value) {
        SerializeVistor<stream_t>::Visit(parser, section, key, value);
    }
};

template<class stream_t>
void INIParser::serialize(stream_t& stream, bool input_order /*= false*/) const {
    assert(line_sep_.length() > 0);
    assert(kv_sep_.length() > 0);
    if (line_sep_.empty() || kv_sep_.empty()) {
        return;
    }

    if (input_order && section_list_.size() == section_map_.size()) {
        SerializeSequenceVistor<stream_t> visitor(&stream);
        Visitor f = std::bind(&SerializeSequenceVistor<stream_t>::visit, &visitor, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        Visit(f, true);
    } else {
        SerializeFastVistor<stream_t> visitor(&stream);
        Visitor f = std::bind(&SerializeFastVistor<stream_t>::visit, &visitor, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        Visit(f, false);
    }
}

void INIParser::Visit(const Visitor& visitor, bool sequential /*= false*/) const {
    if (sequential) {
        SectionList::const_iterator it_section_list(section_list_.begin());
        SectionList::const_iterator ite_section_list(section_list_.end());
        for (; it_section_list != ite_section_list; ++it_section_list) {
            SectionMap::const_iterator it_section_map(section_map_.find(it_section_list->first));
            assert(it_section_map != section_map_.end());
            StringList::const_iterator it_key(it_section_list->second.begin());
            StringList::const_iterator ite_key(it_section_list->second.end());
            for (; it_key != ite_key; ++it_key) {
                ssmap::const_iterator it_map = it_section_map->second.find(*(it_key));
                assert(it_map != it_section_map->second.end());
                visitor(*this, it_section_list->first, *it_key, it_map->second);
            }
        }
        return;
    }

    SectionMap::const_iterator it_sectioin(section_map_.begin());
    SectionMap::const_iterator ite_sectioin(section_map_.end());
    for (; it_sectioin != ite_sectioin; ++it_sectioin) {
        ssmap::const_iterator it(it_sectioin->second.begin());
        ssmap::const_iterator ite(it_sectioin->second.end());
        for (; it != ite; ++it) {
            visitor(*this, it_sectioin->first, it->first, it->second);
        }
    }
}

void INIParser::Visit(const string& section, const Visitor& visitor, bool sequential /*= false*/) const {
    if (sequential) {
        SectionList::const_iterator it_section_list(section_list_.begin());
        SectionList::const_iterator ite_section_list(section_list_.end());
        for (; it_section_list != ite_section_list; ++it_section_list) {
            if (it_section_list->first != section) {
                continue;
            }

            SectionMap::const_iterator it_section_map(section_map_.find(it_section_list->first));
            assert(it_section_map != section_map_.end());
            StringList::const_iterator it_key(it_section_list->second.begin());
            StringList::const_iterator ite_key(it_section_list->second.end());
            for (; it_key != ite_key; ++it_key) {
                ssmap::const_iterator it_map = it_section_map->second.find(*(it_key));
                assert(it_map != it_section_map->second.end());
                visitor(*this, it_section_list->first, *it_key, it_map->second);
            }
        }
    } else {
        SectionMap::const_iterator it_sectioin(section_map_.find(section));
        if (it_sectioin != section_map_.end()) {
            ssmap::const_iterator it(it_sectioin->second.begin());
            ssmap::const_iterator ite(it_sectioin->second.end());
            for (; it != ite; ++it) {
                visitor(*this, it_sectioin->first, it->first, it->second);
            }
        }
    }
}

const char* INIParser::strerror() const {
    H_CASE_STRING_BIGIN(error());
    H_CASE_STRING(kNoError);
    H_CASE_STRING(kErrorCannotFindKVSeperator);
    H_CASE_STRING(kErrorCannotFindLineSeperator);
    H_CASE_STRING(kErrorCannotFindKVSeperatorAndLineSeperator);
    H_CASE_STRING(kErrorSectionFormatWrong);
    H_CASE_STRING_END();
}

wcpp::INIParser::Status INIParser::CheckStatus(const char* dataend, const char* kv_sep_pos, const char*& line_end) {
    /**  Fix the following ini-text, we should find key : 'k2'
         "k1=a\n"
         "k2=b"               ---> processing this line
    */
    if ((!line_end || line_end > (dataend - line_sep_.size())) && (kv_sep_pos && kv_sep_pos < dataend)) {
        line_end = dataend;
        return kFoundKeyAndValue;
    }

    /**  Fix the following ini-text, we should find key : 'k2'
         "k1=a\n"
         "k2=b\n"
         "xxx"       ---> processing this line
    */
    if ((!line_end || line_end > (dataend - line_sep_.size())) && (!kv_sep_pos || kv_sep_pos >= line_end)) {
        line_end = dataend;
        error_code_ = kErrorCannotFindKVSeperator;
        if (compatible_) {
            return kFinishedOK;
        } else {
            return kFinishedError;
        }
    }

    /**  Fix the following ini-text, we should find key : 'k2'
         "k1=a\n"
         "k2=b\n"
         "xxx\n"         ---> processing this line
         "k3=c\n"
    */
    if (line_end && kv_sep_pos >= line_end && kv_sep_pos < dataend) {
        error_code_ = kErrorCannotFindKVSeperator;
        if (compatible_) {
            //try to read next line
            return kNeedSkipThisLine;
        } else {
            return kFinishedError;
        }
    }

    /**  Fix the following ini-text, we should find key : 'k2'
         "k1=a\n"
         "k2=b\n"
         "xxx\n"       ---> processing this line
    */
    if ((line_end && line_end <= (dataend - line_sep_.size())) && (!kv_sep_pos || kv_sep_pos >= line_end)) {
        error_code_ = kErrorCannotFindKVSeperator;
        if (compatible_) {
            return kFinishedOK;
        } else {
            return kFinishedError;
        }
    }

    return kFoundKeyAndValue;
}

void INIParser::set_trim_chars(const std::string& trimchars) {
    trim_chars_ = trimchars;
    InitTrimCharsTable();
}

const char* INIParser::SkipTrimChars(const char* szsrc, const char* end) {
    const uint8_t* t = (const unsigned char*)szsrc;
    for (; t && t < (const uint8_t*)end && trim_chars_table_[*t]; ++t) {
        ;//Do nothing
    }
    return (const char*)t;
}

void INIParser::InitTrimCharsTable() {
    if (trim_chars_.empty()) {
        return;
    }

    const uint8_t* p = (const uint8_t*)trim_chars_.data();
    const uint8_t* end = p + trim_chars_.size();
    for (; p < end; ++p) {
        trim_chars_table_[*p] = 1;
    }
}
}


