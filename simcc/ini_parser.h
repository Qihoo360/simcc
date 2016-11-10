#pragma once

#include "simcc/inner_pre.h"
#include "simcc/gcc_list.h"

#include <string>
#include <map>
#include <vector>


namespace simcc {

class DataStream;

// @brief The class for parsing an INI format file or memory data.
// The INI file can contains comments lines which start with '#' or '//' or ';'
class SIMCC_EXPORT INIParser {
public:
    typedef std::map<string, string> ssmap;
    typedef std::map<string, ssmap> SectionMap;

    typedef list<string> StringList;
    typedef std::pair<string, StringList> SectionPairEntry;
    typedef list<SectionPairEntry> SectionList;

    typedef std::function<void(INIParser& parser, const string& section, const string& key, const string& value)> ParseListener;
    typedef std::function<void(const INIParser& parser, const string& section, const string& key, const string& value)> Visitor;

    enum ErrorCode {
        kNoError = 0,
        kErrorCannotFindKVSeperator = 1,
        kErrorCannotFindLineSeperator = 2,
        kErrorCannotFindKVSeperatorAndLineSeperator = 3,
        kErrorSectionFormatWrong = 4,
    };
public:
    // @brief
    // @param[in] is_case_sensitive -
    // @param[in] is_compatible - If it is true, we will neglect the wrong format line and try to parse the remaining contents
    // @param[in] keep_sequence - If it is true, we will record the sequence of the input INI file. This will slow down the parsing speed.
    // @return  -
    INIParser(bool is_case_sensitive = true, bool is_compatible = false, bool keep_sequence = false);

    ~INIParser();

    // @brief Clear the section/key-value map
    void Reset();

    // @brief Parse the INI file
    // @param const string & filename - The INI file name
    // @return bool - return true if parse successfully.
    //     In the compatible mode of parsing, even this function return true, 
    // there is a chance that the INI file contents is wrong, 
    // at this time we can use <code>error()</code> function to get the actual error code
    bool Parse(const string& filename);
    bool Parse(const char* filename);

    // @brief Parse the raw data.
    //     We will parse the data every line.
    //   Lines are separated by '\n'.
    // @param const char * data - The source data
    // @param size_t datalen - The length of the data
    // @return bool - return true if parse successfully
    bool Parse(const char* data, size_t datalen);

    // @brief Parse the raw data
    //     We will parse the data line by line.
    // Lines are separated by <code>linesep</code>
    // key-value is separated by <code>keyvaluesep</code>
    // @param const char * data -
    // @param size_t datalen -
    // @param const char * linesep - the line separator
    // @param const char * keyvaluesep - the key/value separator
    // @return bool -
    bool Parse(const char* data, size_t datalen, const char* linesep, const char* keyvaluesep);
    bool Parse(const char* data, size_t datalen, const string& linesep, const string& keyvaluesep);

    // @brief When parsing the INI file, you can call this function to stop the parsing
    // @return void -
    void StopParsing(bool stop_parsing);

    //The error code
    ErrorCode error() const {
        return error_code_;
    }
    bool ok() const {
        return error_code_ == kNoError;
    }
    const char* strerror() const;

    // @brief
    // @param ParseListener * pl -
    // @return void -
    void SetParseListener(ParseListener pl);

    // Query
public:
    // @brief Get the value of specified key from the default section
    // @param const string & key -
    // @param[out] size_t * data_len - If it is provided, the value length will stored here
    // @return const char* - If find return the value, or return NULL
    // @return const string& - If find return the value, or return an empty string
    const string& Get(const string& key, bool* found = NULL) const;

    // @brief Get the value of specified key from a certain section
    // @param const string & section - The section name
    // @param const string & key - The key
    // @param[out] size_t * data_len - If it is provided, the value length will stored here
    // @return const char* - If find return the value, or return NULL
    // @return const string& - If find return the value, or return an empty string
    const string& Get(const string& section, const string& key, bool* found = NULL) const;

    int64_t GetInteger(const string& key, int64_t default_value = 0) const;
    int64_t GetInteger(const string& section, const string& key, int64_t default_value = 0) const;

    bool GetBool(const string& key, bool default_value = false) const;
    bool GetBool(const string& section, const string& key, bool default_value = false) const;

    double GetDouble(const string& key, double default_value = 0.0) const;
    double GetDouble(const string& section, const string& key, double default_value = 0.0) const;

    // @brief Set the value of specified key into a certain section
    // @param const string & section - The section name
    // @param const string & key - The key
    // @param const string & key - The value
    void Set(const string& section, const string& key, const string& value);
    void Set(const string& key, const string& value);

    // @brief Get the default key/value Map
    //     which equals to call <code>GetKeyValueMap("")</code>
    // @return const StringAStringAMap& -
    const ssmap& GetDefaultKeyValueMap();

    // @brief Get the key/value Map of a certain section
    // @param const string & section - The section name
    // @return const ssmap& -
    const ssmap& GetKeyValueMap(const string& section) const;

    // @brief Get the section full map
    // @return - const SectionMap&
    const SectionMap& GetSectionMap() const {
        return section_map_;
    }

public:
    bool compatible() const {
        return compatible_;
    }
    void set_compatible(bool val) {
        compatible_ = val;
    }

    // @brief Query whether it is case sensitive
    // @return - bool
    bool case_sensitive() const {
        return case_sensitive_;
    }
    void set_case_sensitive(bool b) {
        case_sensitive_ = b;
    }
    void set_trim_chars(const string& trim_chars);
    void set_keep_sequence(bool b) {
        keep_sequence_ = b;
    }

    // Separator
public:
    void SetKVSeparator(const string& separator) {
        kv_sep_  = separator;
    }
    void SetLineSeparator(const string& separator) {
        line_sep_  = separator;
    }
    const string& line_separator() const {
        return line_sep_;
    }
    const string& kv_separator() const {
        return kv_sep_;
    }

    // Visit
public:

    // @brief Visit all the section/key/value in this INI.
    //     If sequential is true, we will visit the section/key/value
    // in the order of the input file.
    //     If sequential is false, we will visit the section/key/value
    // in the memory order, it is may be not the same of the input order.
    // @param[in] - FastVisitor & visitor
    // @return - void
    void Visit(const Visitor& visitor, bool sequential = false) const;
    void Visit(const string& section, const Visitor& visitor, bool sequential = false) const;

    // Serialize
public:
    // @brief Serialize the INI to output string
    // @param[in] - bool input_order - true if we want to
    //     serialize this INI object as the order of the input INI file
    // @return - string
    string Serialize(bool input_order = false) const;

    // @brief Serialize the INI to output string
    // @param[in] - string & output
    // @param[in] - MemoryDataStream & buf
    // @param[in] - std::ostream & os
    // @return - void
    void Serialize(string& output, bool input_order = false) const;
    void Serialize(DataStream& buf, bool input_order = false) const;
    void Serialize(std::ostream& os, bool input_order = false) const;

private:
    void InitTrimCharsTable();

    // @brief Skip the lines which start with '#' or '//' or ";"
    // @note Lines are separated by '\n' or '\r\n'
    // @param const char * szsrc - The source string
    // @return const char* - The pointer to the next line
    const char* SkipCommits(const char* szsrc, size_t len);

    // @brief Skipping whitespace, such as '\n' '\r' ' '
    // @param const char * szsrc - The source string
    // @return const char* - The pointer to the next character
    const char* SkipSpaces(const char* szsrc);

    // @brief Skipping the characters within <code>trim_chars_</code>
    // @param const char * szsrc - The source string
    // @param const char * dataend - The end of the string, not include
    // @return const char* - The pointer to the next character
    const char* SkipTrimChars(const char* szsrc, const char* dataend);

    template<class stream_t>
    void serialize(stream_t& os, bool input_order = false) const;

    enum Status {
        kFinishedError = 0,
        kFinishedOK,
        kFoundKeyAndValue,
        kNeedSkipThisLine,
    };

    Status CheckStatus(const char* dataend, const char* kv_sep_pos, const char*& line_end/*in,out*/);

private:
    bool case_sensitive_;// case sensitive flag
    bool stop_parsing_;  // stop the parsing
    bool keep_sequence_;
    SectionMap section_map_; // pair<section string, key/value map>
    string kv_sep_;// The key/value separator
    string line_sep_;// The key/value separator
    SectionList section_list_; // pair<section string, keys list>

    ErrorCode error_code_;
    bool compatible_;

    string trim_chars_;  // the trimming characters when parsing
    char trim_chars_table_[256];

    typedef std::list<ParseListener> ListenerList;
    ListenerList listeners_;

    class SequenceParseListener;
};

inline void INIParser::Reset() {
    section_map_.clear();
}

inline void INIParser::StopParsing(bool stop_parsing) {
    stop_parsing_ = stop_parsing;
}

}
