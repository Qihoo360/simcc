// Created: 2009/08/07
// Author: weizili
#pragma once

#include "wcpp/inner_pre.h"
#include "wcpp/ref_object.h"
#include "wcpp/string_util.h"
#include "wcpp/data_stream.h"

namespace wcpp {
namespace json {

enum JSONType {
    kUnknownType = 0,
    kJSONObject,  // JSONObject
    kJSONArray,   // JSONArray
    kJSONString,  // JSONString
    kJSONInteger, // JSONInteger
    kJSONDouble,  // JSONDouble
    kJSONBoolean, // JSONBoolean
    kJSONNull,    // JSONNull

    kTypeForce = 0xFF // bitwise-and with mask of 0xFF can be optimized by compiler
};

class JSONBoolean;
class JSONInteger;
class JSONDouble;
class JSONString;
class JSONArray;
class JSONObject;

class JSONTokener;

template<typename _Type> struct ToJSONType;

template<>
struct ToJSONType<bool> {
    typedef JSONBoolean JSONClass;
    enum { Type = kJSONBoolean };
};

template<>
struct ToJSONType<wcpp::int64> {
    typedef JSONInteger JSONClass;
    enum { Type = kJSONInteger };
};

template<>
struct ToJSONType<wcpp::float64> {
    typedef JSONDouble JSONClass;
    enum { Type = kJSONDouble };
};

template<>
struct ToJSONType<std::string> {
    typedef JSONString JSONClass;
    enum { Type = kJSONString };
};

template<>
struct ToJSONType<JSONArray> {
    typedef JSONArray JSONClass;
    enum { Type = kJSONArray };
};

template<>
struct ToJSONType<JSONObject> {
    typedef JSONObject JSONClass;
    enum { Type = kJSONObject };
};

//  Abstract class of all json Objects :
//  JSONObject, JSONArray, JSONString, JSONInteger, JSONDouble, JSONBoolean, JSONNull
class WCPP_EXPORT Object : public wcpp::RefObject {
public:
    enum { Type = kUnknownType };
    Object(JSONType e)
        : type_(e) {}

    virtual ~Object() {}

    JSONType type() const {
        return type_;
    }

    bool IsTypeOf(JSONType ot) const {
        return (ot == type_);
    }

    // @brief
    // @param[in] readable, true, to make a pretty printed JSON text of this object.
    //           false, to make a compact JSON text, no whitespace is added.
    // @param[in] utf8_to_unicode - true to guaranteed to consist only of ASCII characters. This is archived by escaping all Unicode characters outside the ASCII range
    // @return the json text string
    std::string ToString(bool readable = false, bool utf8_to_unicode = true) const;

    // Make a JSON text of an Object value. the result is stored in outs
    //   e.g. string "abc" => "\"abc\""
    //   e.g. string "ab\t" => "\"ab\\t\""
    //   e.g. int 324 =>  "324"
    // @param[out] outs -  the json text string
    // @param[in] readable - true, to make a pretty printed JSON text of this object.
    //           false, to make a compact JSON text, no whitespace is added.
    // @param[in] utf8_to_unicode - true to guaranteed to consist only of ASCII characters. This is achieved by escaping all Unicode characters outside the ASCII range
    // @return true, if success, or false
    virtual void ToString(std::string& outs, bool readable = false, bool utf8_to_unicode = true) const = 0;
    virtual void ToStringBuf(wcpp::DataStream& sb, size_t indent = 0, bool utf8_to_unicode = true) const = 0;

    // @warning This method is quite not effective, so make sure why you need call this method
    // @note We mostly use this method to do some unit test
    // @return true if rhs is the same type and the value is equal to each other
    virtual bool Equals(const Object& rhs) = 0;

protected:
    // Load, Deserialize. load from data stream to convert it to a json object
    //     Before we call this function, we has deserialized the a wcpp::u8
    // as the type, so we just deserialize the data in this function
    // and don't care about the first wcpp::uint8 type.
    virtual bool LoadFrom(wcpp::DataStream& file) = 0;

    // Save, Serializer. save the json object in the data stream
    // we first save the type as a wcpp::uint8 in the file and then save the data.
    virtual void SaveTo(wcpp::DataStream& file) const = 0;

protected:
    // Deserialize the data stream to parse one json object.
    // First we deserialize a type and then deserialize an object according to the type.
    // @param file[in] the binary data stream
    // @param pObject[out] the parsed json object
    // @return true, if load a json object successfully, you need to take over the memory delete of pObject, or false.
    static bool DeserializeOneObject(wcpp::DataStream& file, Object*& pObject);

private:
    JSONType  type_;

private:
    // we need to access SaveTo, LoadFrom
    friend class JSONArray;
    friend class JSONObject;

    Object();
};

typedef wcpp::RefPtr<Object> ObjectPtr;

}
}




