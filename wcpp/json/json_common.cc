#include "wcpp/string_util.h"
#include "wcpp/data_stream.h"
#include "wcpp/utility.h"

#include "json.h"

namespace wcpp {
namespace json {

bool Object::DeserializeOneObject(wcpp::DataStream& file, Object*& o) {
    wcpp::uint8 type = kTypeForce;
    file >> type;
    switch (type) {
    case kJSONObject:
        o = new JSONObject;
        break;
    case kJSONArray:
        o = new JSONArray;
        break;
    case kJSONString:
        o = new JSONString;
        break;
    case kJSONInteger:
        o = new JSONInteger;
        break;
    case kJSONDouble:
        o = new JSONDouble;
        break;
    case kJSONBoolean:
        o = new JSONBoolean;
        break;
    case kJSONNull:
        o = new JSONNull;
        break;
    default:
        assert(false);
        return false;
    }

    if (o) {
        return o->LoadFrom(file);
    }

    return false;
}


std::string Object::ToString(bool readable, bool utf8_to_unicode) const {
    std::string  retVal;
    this->ToString(retVal, readable, utf8_to_unicode);
    return retVal;
}

}
}
