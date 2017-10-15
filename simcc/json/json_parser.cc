#include "simcc/string_util.h"
#include "simcc/data_stream.h"
#include "simcc/utility.h"
#include "json.h"
#include "json_tokener.h"

namespace simcc {
namespace json {

JSONParser::JSONParser()
    : buf_(1024)
    , error_code_(kNoError)
    , error_location_(0) {
}

void JSONParser::set_error(ErrorCode ec, JSONTokener* x) {
    error_code_ = ec;
    if (x) {
        x->Back();
        error_location_ = x->GetCurrentPosition();
    }
}


void JSONParser::set_error(ErrorCode ec, size_t location) {
    error_code_ = ec;
    error_location_ = location;
}

const char* JSONParser::strerror() const {
    H_CASE_STRING_BIGIN(error());
    H_CASE_STRING(kNoError);
    H_CASE_STRING(kParameterWrong);
    H_CASE_STRING(kCommentFormatError);
    H_CASE_STRING(kJSONArrayNotBeginWithBrackets);
    H_CASE_STRING(kJSONArrayNotEndWithBrackets);
    H_CASE_STRING(kJSONObjectNotBeginWithBraces);
    H_CASE_STRING(kJSONObjectNotEndWithBraces);
    H_CASE_STRING(kJSONObjectKeyNotString);
    H_CASE_STRING(kJSONObjectKeyIsEmpty);
    H_CASE_STRING(kInvalidCharacter);
    H_CASE_STRING(kKeyValueSeperatorError);
    H_CASE_STRING(kJSONStringNotQuoted);
    H_CASE_STRING(kBlankValue);
    H_CASE_STRING(kInvalidIntegerOrDoubleString);
    H_CASE_STRING(kInvalidHexadecimalCharacter);
    H_CASE_STRING(kInvalidOctalCharacter);
    H_CASE_STRING(kDeserializeBinaryDataError);
    H_CASE_STRING(kLoadBinaryDataError);
    H_CASE_STRING_END();
}

ObjectPtr JSONParser::LoadFile(const string& json_file_path) {
    simcc::DataStream ds;
    if (!ds.ReadFile(json_file_path)) {
        return nullptr;
    }

    return Load(ds.data(), ds.size());
}

ObjectPtr JSONParser::Load(const char* source, const simcc::int64 source_len /*= -1 */) {
    if (source_len == 0 || !source) {
        return nullptr;
    }

    json::JSONTokener x(source, source_len);
    if (!x.SkipComment()) {
        return nullptr;
    }

    char c = x.NextClean();
    if (c == '{') {
        JSONObjectPtr jo = new JSONObject();
        x.Back();
        if (jo->Parse(&x) && jo->error() == kNoError) {
            return jo.get();
        }
        return nullptr;
    } else if (c == '[') {
        JSONArrayPtr ja = new JSONArray();
        x.Back();
        if (ja->Parse(&x) && ja->error() == kNoError) {
            return ja.get();
        }
        return nullptr;
    }

    return nullptr;
}

}
}
