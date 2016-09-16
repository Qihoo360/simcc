// Created: 2009/08/07
// Author: weizili
#pragma once

#include "simcc/inner_pre.h"
#include "simcc/ref_object.h"
#include "simcc/string_util.h"
#include "simcc/data_stream.h"

namespace simcc {
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
struct ToJSONType<simcc::int64> {
    typedef JSONInteger JSONClass;
    enum { Type = kJSONInteger };
};

template<>
struct ToJSONType<simcc::float64> {
    typedef JSONDouble JSONClass;
    enum { Type = kJSONDouble };
};

template<>
struct ToJSONType<string> {
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
class SIMCC_EXPORT Object : public simcc::RefObject {
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
    string ToString(bool readable = false, bool utf8_to_unicode = true) const;

    // Make a JSON text of an Object value. the result is stored in outs
    //   e.g. string "abc" => "\"abc\""
    //   e.g. string "ab\t" => "\"ab\\t\""
    //   e.g. int 324 =>  "324"
    // @param[out] outs -  the json text string
    // @param[in] readable - true, to make a pretty printed JSON text of this object.
    //           false, to make a compact JSON text, no whitespace is added.
    // @param[in] utf8_to_unicode - true to guaranteed to consist only of ASCII characters. This is achieved by escaping all Unicode characters outside the ASCII range
    // @return true, if success, or false
    virtual void ToString(string& outs, bool readable = false, bool utf8_to_unicode = true) const = 0;
    virtual void ToStringBuf(simcc::DataStream& sb, size_t indent = 0, bool utf8_to_unicode = true) const = 0;

    // @warning This method is quite not effective, so make sure why you need call this method
    // @note We mostly use this method to do some unit test
    // @return true if rhs is the same type and the value is equal to each other
    virtual bool Equals(const Object& rhs) = 0;

protected:
    // Load, Deserialize. load from data stream to convert it to a json object
    //     Before we call this function, we has deserialized the a simcc::u8
    // as the type, so we just deserialize the data in this function
    // and don't care about the first simcc::uint8 type.
    virtual bool LoadFrom(simcc::DataStream& file) = 0;

    // Save, Serializer. save the json object in the data stream
    // we first save the type as a simcc::uint8 in the file and then save the data.
    virtual void SaveTo(simcc::DataStream& file) const = 0;

protected:
    // Deserialize the data stream to parse one json object.
    // First we deserialize a type and then deserialize an object according to the type.
    // @param file[in] the binary data stream
    // @param pObject[out] the parsed json object
    // @return true, if load a json object successfully, you need to take over the memory delete of pObject, or false.
    static bool DeserializeOneObject(simcc::DataStream& file, Object*& pObject);

private:
    JSONType  type_;

private:
    // we need to access SaveTo, LoadFrom
    friend class JSONArray;
    friend class JSONObject;

    Object();
};

typedef simcc::RefPtr<Object> ObjectPtr;

}
}




