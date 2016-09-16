// Created: 2009/08/07
// Author: weizili

#pragma once

#include "simcc/inner_pre.h"
#include "json_common.h"

namespace simcc {
namespace json {

template<class ValueType>
class JSONValue : public Object {
public:
    JSONValue()
        : Object(static_cast<JSONType>(ToJSONType<ValueType>::Type))
        , value_(ValueType()) {
    }
    JSONValue(const ValueType& v)
        : Object(static_cast<JSONType>(ToJSONType<ValueType>::Type))
        , value_(v) {
    }

    const ValueType& value() const {
        return value_;
    }

    ValueType& value() {
        return value_;
    }

    void set_value(const ValueType& newvalue) {
        value_ = newvalue;
    }

    operator ValueType& () {
        return value_;
    }

protected:
    ValueType value_;
};


// The concrete object of JSON : JSONInteger
class SIMCC_EXPORT JSONInteger : public JSONValue<simcc::int64> {
    typedef JSONValue<simcc::int64> BaseClass;
public:
    enum { Type = kJSONInteger };
    JSONInteger(simcc::int64 v = 0);
    JSONInteger& operator=(simcc::int64 v);

    bool operator==(const JSONInteger& rhs) {
        return value_ == rhs.value_;
    }
    bool operator==(simcc::int64 v) {
        return value_ == v;
    }

    using Object::ToString; // string ToString(bool readable = false, bool utf8_to_unicode = true) const;
    virtual void ToString(string& s, bool readable = false, bool utf8_to_unicode = true)const;
    virtual void ToStringBuf(simcc::DataStream& sb, size_t indent = 0, bool utf8_to_unicode = true)const;

    // @warning This method is quite not effective, so make sure why you need call this method
    // @note We mostly use this method to do some unit test
    // @return true If rhs is the same type and the value is equal to each other
    virtual bool Equals(const Object& rhs);

private:
    // override method from base class json::Object
    virtual bool LoadFrom(simcc::DataStream& file);
    virtual void SaveTo(simcc::DataStream& file) const;
};

// The concrete object of JSON : JSONDouble
class SIMCC_EXPORT JSONDouble : public JSONValue<simcc::float64> {
    typedef JSONValue<simcc::float64> BaseClass;
public:
    enum { Type = kJSONDouble };
    JSONDouble(simcc::float64 value = 0);
    JSONDouble& operator=(simcc::float64 value);

    bool Equals(const JSONDouble& other, simcc::float64 tolarence = 0.00001);
    bool Equals(simcc::float64 value, simcc::float64 tolarence = 0.00001);

    using Object::ToString; // string ToString(bool readable = false, bool utf8_to_unicode = true) const;
    virtual void ToString(string& s, bool readable = false, bool utf8_to_unicode = true)const;
    virtual void ToStringBuf(simcc::DataStream& sb, size_t indent = 0, bool utf8_to_unicode = true)const;

    // @warning This method is quite not effective, so make sure why you need call this method
    // @note We mostly use this method to do some unit test
    // @return true If rhs is the same type and the value is equal to each other
    virtual bool Equals(const Object& rhs);
private:
    // override method from base class json::Object
    virtual bool LoadFrom(simcc::DataStream& file);
    virtual void SaveTo(simcc::DataStream& file) const;
};

// The concrete object of JSON : JSONBoolean
class SIMCC_EXPORT JSONBoolean : public JSONValue<bool> {
    typedef JSONValue<bool> BaseClass;
public:
    enum { Type = kJSONBoolean };
    JSONBoolean(bool value = false);
    JSONBoolean& operator=(bool value);
    bool operator==(const JSONBoolean& rhs) {
        return value_ == rhs.value_;
    }
    bool operator==(bool v) {
        return value_ == v;
    }

    using Object::ToString; // string ToString(bool readable = false, bool utf8_to_unicode = true) const;
    virtual void ToString(string& s, bool readable = false, bool utf8_to_unicode = true) const;
    virtual void ToStringBuf(simcc::DataStream& sb, size_t indent = 0, bool utf8_to_unicode = true) const;

    // @warning This method is quite not effective, so make sure why you need call this method
    // @note We mostly use this method to do some unit test
    // @return true If rhs is the same type and the value is equal to each other
    virtual bool Equals(const Object& rhs);
private:
    // override method from base class json::Object
    virtual bool LoadFrom(simcc::DataStream& file);
    virtual void SaveTo(simcc::DataStream& file) const;
};

// The concrete object of JSON : JSONString
class SIMCC_EXPORT JSONString : public JSONValue<string> {
    typedef JSONValue<string> BaseClass;
public:
    enum { Type = kJSONString };
    JSONString(const string& value);
    JSONString(const char* value = "");
    JSONString& operator=(const string& value);
    bool operator==(const JSONString& rhs) {
        return value_ == rhs.value_;
    }
    bool operator==(const string& v) {
        return value_ == v;
    }
    bool operator==(const char* v) {
        return value_ == v;
    }

    using JSONValue<string>::set_value;
    void set_value(const char* v) {
        value_ = v;
    }

    using Object::ToString; // string ToString(bool readable = false, bool utf8_to_unicode = true) const;
    virtual void ToString(string& s, bool readable = false, bool utf8_to_unicode = true)const;
    virtual void ToStringBuf(simcc::DataStream& sb, size_t indent = 0, bool utf8_to_unicode = true)const;

    // @warning This method is quite not effective, so make sure why you need call this method
    // @note We mostly use this method to do some unit test
    // @return true If rhs is the same type and the value is equal to each other
    virtual bool Equals(const Object& rhs);
private:
    // override method from base class json::Object
    virtual bool LoadFrom(simcc::DataStream& file);
    virtual void SaveTo(simcc::DataStream& file) const;

};


class SIMCC_EXPORT JSONNull : public Object {
public:
    JSONNull() : Object(kJSONNull) {}

    using Object::ToString; // string ToString(bool readable = false, bool utf8_to_unicode = true) const;
    virtual void ToString(string& s, bool readable = false, bool utf8_to_unicode = true)const;
    virtual void ToStringBuf(simcc::DataStream& sb, size_t indent = 0, bool utf8_to_unicode = true)const;

    // @warning This method is quite not effective, so make sure why you need call this method
    // @note We mostly use this method to do some unit test
    // @return true If rhs is the same type and the value is equal to each other
    virtual bool Equals(const Object& rhs);
private:
    // override method from base class json::Object
    virtual bool LoadFrom(simcc::DataStream& file);
    virtual void SaveTo(simcc::DataStream& file) const;

};


}
}

