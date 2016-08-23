// Created: 2009/08/07
// Author: weizili

#pragma once

#include "wcpp/inner_pre.h"
#include "json_common.h"

namespace wcpp {
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
class WCPP_EXPORT JSONInteger : public JSONValue<wcpp::int64> {
    typedef JSONValue<wcpp::int64> BaseClass;
public:
    enum { Type = kJSONInteger };
    JSONInteger(wcpp::int64 v = 0);
    JSONInteger& operator=(wcpp::int64 v);

    bool operator==(const JSONInteger& rhs) {
        return value_ == rhs.value_;
    }
    bool operator==(wcpp::int64 v) {
        return value_ == v;
    }

    using Object::ToString; // std::string ToString(bool readable = false, bool utf8_to_unicode = true) const;
    virtual void ToString(std::string& s, bool readable = false, bool utf8_to_unicode = true)const;
    virtual void ToStringBuf(wcpp::DataStream& sb, size_t indent = 0, bool utf8_to_unicode = true)const;

    // @warning This method is quite not effective, so make sure why you need call this method
    // @note We mostly use this method to do some unit test
    // @return true If rhs is the same type and the value is equal to each other
    virtual bool Equals(const Object& rhs);

private:
    // override method from base class json::Object
    virtual bool LoadFrom(wcpp::DataStream& file);
    virtual void SaveTo(wcpp::DataStream& file) const;
};

// The concrete object of JSON : JSONDouble
class WCPP_EXPORT JSONDouble : public JSONValue<wcpp::float64> {
    typedef JSONValue<wcpp::float64> BaseClass;
public:
    enum { Type = kJSONDouble };
    JSONDouble(wcpp::float64 value = 0);
    JSONDouble& operator=(wcpp::float64 value);

    bool Equals(const JSONDouble& other, wcpp::float64 tolarence = 0.00001);
    bool Equals(wcpp::float64 value, wcpp::float64 tolarence = 0.00001);

    using Object::ToString; // std::string ToString(bool readable = false, bool utf8_to_unicode = true) const;
    virtual void ToString(std::string& s, bool readable = false, bool utf8_to_unicode = true)const;
    virtual void ToStringBuf(wcpp::DataStream& sb, size_t indent = 0, bool utf8_to_unicode = true)const;

    // @warning This method is quite not effective, so make sure why you need call this method
    // @note We mostly use this method to do some unit test
    // @return true If rhs is the same type and the value is equal to each other
    virtual bool Equals(const Object& rhs);
private:
    // override method from base class json::Object
    virtual bool LoadFrom(wcpp::DataStream& file);
    virtual void SaveTo(wcpp::DataStream& file) const;
};

// The concrete object of JSON : JSONBoolean
class WCPP_EXPORT JSONBoolean : public JSONValue<bool> {
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

    using Object::ToString; // std::string ToString(bool readable = false, bool utf8_to_unicode = true) const;
    virtual void ToString(std::string& s, bool readable = false, bool utf8_to_unicode = true) const;
    virtual void ToStringBuf(wcpp::DataStream& sb, size_t indent = 0, bool utf8_to_unicode = true) const;

    // @warning This method is quite not effective, so make sure why you need call this method
    // @note We mostly use this method to do some unit test
    // @return true If rhs is the same type and the value is equal to each other
    virtual bool Equals(const Object& rhs);
private:
    // override method from base class json::Object
    virtual bool LoadFrom(wcpp::DataStream& file);
    virtual void SaveTo(wcpp::DataStream& file) const;
};

// The concrete object of JSON : JSONString
class WCPP_EXPORT JSONString : public JSONValue<std::string> {
    typedef JSONValue<std::string> BaseClass;
public:
    enum { Type = kJSONString };
    JSONString(const std::string& value);
    JSONString(const char* value = "");
    JSONString& operator=(const std::string& value);
    bool operator==(const JSONString& rhs) {
        return value_ == rhs.value_;
    }
    bool operator==(const std::string& v) {
        return value_ == v;
    }
    bool operator==(const char* v) {
        return value_ == v;
    }

    using JSONValue<std::string>::set_value;
    void set_value(const char* v) {
        value_ = v;
    }

    using Object::ToString; // std::string ToString(bool readable = false, bool utf8_to_unicode = true) const;
    virtual void ToString(std::string& s, bool readable = false, bool utf8_to_unicode = true)const;
    virtual void ToStringBuf(wcpp::DataStream& sb, size_t indent = 0, bool utf8_to_unicode = true)const;

    // @warning This method is quite not effective, so make sure why you need call this method
    // @note We mostly use this method to do some unit test
    // @return true If rhs is the same type and the value is equal to each other
    virtual bool Equals(const Object& rhs);
private:
    // override method from base class json::Object
    virtual bool LoadFrom(wcpp::DataStream& file);
    virtual void SaveTo(wcpp::DataStream& file) const;

};


class WCPP_EXPORT JSONNull : public Object {
public:
    JSONNull() : Object(kJSONNull) {}

    using Object::ToString; // std::string ToString(bool readable = false, bool utf8_to_unicode = true) const;
    virtual void ToString(std::string& s, bool readable = false, bool utf8_to_unicode = true)const;
    virtual void ToStringBuf(wcpp::DataStream& sb, size_t indent = 0, bool utf8_to_unicode = true)const;

    // @warning This method is quite not effective, so make sure why you need call this method
    // @note We mostly use this method to do some unit test
    // @return true If rhs is the same type and the value is equal to each other
    virtual bool Equals(const Object& rhs);
private:
    // override method from base class json::Object
    virtual bool LoadFrom(wcpp::DataStream& file);
    virtual void SaveTo(wcpp::DataStream& file) const;

};


}
}

