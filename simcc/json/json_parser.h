// Created: 2009/08/07
// Author: weizili
#pragma once

#include "simcc/inner_pre.h"
#include "json_common.h"

namespace simcc {
namespace json {

class SIMCC_EXPORT JSONParser {
public:
    enum ErrorCode {
        kNoError = 0,
        kParameterWrong,
        kCommentFormatError,
        kJSONArrayNotBeginWithBrackets, //A JSONArray text must start with '['
        kJSONArrayNotEndWithBrackets, //A JSONArray text must end with ']'

        kJSONObjectNotBeginWithBraces, //A JSONObject text must start with '{'
        kJSONObjectNotEndWithBraces, //A JSONObject text must start with '}'
        kJSONObjectKeyNotString, //A JSONObject key not a string
        kJSONObjectKeyIsEmpty,

        kInvalidCharacter,
        kKeyValueSeperatorError, //The key/value separator not a ':'

        kJSONStringNotQuoted, //A JOSNString must be quoted by ""
        kBlankValue, //A JSON key followed by a blank string, no value
        kInvalidIntegerOrDoubleString, //A invalid JSONInteger or JSONDouble string

        kInvalidHexadecimalCharacter, //Not a valid hexadecimal character
        kInvalidOctalCharacter, //Not a valid octal character

        kDeserializeBinaryDataError,
        kLoadBinaryDataError,
    };
public:
    JSONParser();

    ErrorCode error() const {
        return error_code_;
    }

    const char* strerror() const;

    bool ok() const {
        return error() == kNoError;
    }

    size_t error_location() const {
        return error_location_;
    }

public:

    // Construct a JSONArray or JSONObject from the source JSON text or file
    // @param source A string of JSON format text
    // @param source_len, the length of the source string.
    //   if you use the default value(-1), we will use strlen(source) to
    //   calculate the length.
    // @return JSONArray* if the <code>source</code> is a JSONArray string
    //      or JSONObject* if the <code>source</code> is a JSONObject string
    //      or NULL if there is some error happened which you can
    //         use error() to get the error code
    static ObjectPtr Load(const char* source, const simcc::int64 source_len = -1);

    static ObjectPtr LoadFile(const std::string& json_file_path);

protected:
    void set_error(ErrorCode ec, JSONTokener* x = NULL);
    void set_error(ErrorCode ec, size_t error_location);

    // This 3 classes will call <code>set_error</code> function
    friend class JSONArray;
    friend class JSONObject;
    friend class JSONTokener;

    mutable simcc::DataStream buf_;// The data cache buffer for the sake of improve performance
private:
    ErrorCode error_code_;
    size_t error_location_;//the offset of the <code>source</code>
};

}
}

