// Created: 2009/08/07
// Author: weizili
#pragma once
#include "simcc/inner_pre.h"

#include "json_common.h"
#include "json_value.h"
#include "json_parser.h"

namespace simcc {
namespace json {
class JSONArray;
class JSONTokener;
class SIMCC_EXPORT JSONObject : public Object, public JSONParser {
public:
    typedef std::map<std::string, ObjectPtr> ObjectPtrMap;
    typedef ObjectPtrMap Map;
    typedef ObjectPtrMap::iterator                  Iterator;
    typedef ObjectPtrMap::const_iterator            ConstIterator;
    typedef Iterator                                iterator;
    typedef ConstIterator                           const_iterator;
    typedef ObjectPtrMap::reverse_iterator          reverse_iterator;
    typedef ObjectPtrMap::const_reverse_iterator    const_reverse_iterator;

public:
    enum { Type = kJSONObject };
    JSONObject() : Object(kJSONObject) {}
    
    // Construct a JSONObject from a source JSON text string.
    // @note Don't use this tow constructors, unless you can make sure
    //   the json text string is valid format. You can use JSONObject()
    //   instead, and use Parse(...) method to build a JSON object
    //   from a json text string.
    // @param source A string beginning
    //  with <code>{</code>&nbsp;<small>(left brace)</small> and ending
    //  with <code>}</code>&nbsp;<small>(right brace)</small>.
    // @param source_len The length of the source string
    //   if it Equals -1, we will use strlen(source) to calculate the length.
    JSONObject(const std::string& source);
    JSONObject(const char* source, const simcc::int32 source_len = -1);

    virtual ~JSONObject();

public:
    // Construct a JSONArray from a source JSON text.
    // @param source     A string that begins with
    // <code>[</code>&nbsp;<small>(left bracket)</small>
    //  and ends with <code>]</code>&nbsp;<small>(right bracket)</small>.
    // @return number of characters parsed. Return 0 if failed to parse.
    simcc::uint32 Parse(const std::string& source);

    // Construct a JSONArray from a source JSON text.
    // @param source     A string that begins with
    // <code>[</code>&nbsp;<small>(left bracket)</small>
    //  and ends with <code>]</code>&nbsp;<small>(right bracket)</small>.
    // @param source_len, the length of the source string.
    //   if you use the default value(-1), we will use strlen(source) to
    //   calculate the length.
    // @return number of characters parsed. Return 0 if failed to parse.
    simcc::uint32 Parse(const char* source, const simcc::int64 source_len = -1);

    using Object::ToString; // std::string ToString(bool readable = false, bool utf8_to_unicode = true) const;
    virtual void ToString(std::string& s, bool readable = false, bool utf8_to_unicode = true) const;
    virtual void ToStringBuf(simcc::DataStream& sb, size_t indent = 0, bool utf8_to_unicode = true) const;

    // @warning This method is quite not effective, so make sure why you need call this method
    // @note We mostly use this method to do some unit test
    // @return true If rhs is the same type and the value is equal to each other
    virtual bool Equals(const Object& rhs);

    // Load, Deserialize. load from data stream to convert it to a json object
    SIMCC_EXPORT friend simcc::DataStream& operator >> (simcc::DataStream& file, JSONObject& val);

    // Save, Serialize. save the json object in the data stream
    SIMCC_EXPORT friend simcc::DataStream& operator << (simcc::DataStream& file, const JSONObject& val);

#ifdef H_PROVIDE_PROTOBUF_DEBUGSTRING_2_JSON
    simcc::uint32 ParseProtobuf(const std::string& protobuf_debug_string);
#endif

public:
    // Try to convert a string into a number, boolean, or null. If the string
    // can't be converted, return NULL
    // @note remember to delete the point , which is returned by this function
    // @param s A String.
    // @return A simple JSON value.
    static Object* ConvertToObject(const char* s, size_t len, JSONParser* parser, JSONTokener* x);

public:
    // Get the object value associated with key value.
    // @param key  the key value
    // @return An object value.
    //   NULL, If there is no value for the index.
    Object*      Get(const std::string& key) const;
    JSONBoolean* GetJSONBoolean(const std::string& key) const;
    JSONDouble*  GetJSONDouble(const std::string& key) const;
    JSONInteger* GetJSONInteger(const std::string& key) const;
    JSONString*  GetJSONString(const std::string& key) const;
    JSONArray*   GetJSONArray(const std::string& key) const;
    JSONObject*  GetJSONObject(const std::string& key) const;

    // Gets a value
    // @param strKey, the key
    // @param default_value, the default value.
    // @return the bool value.
    //   or default_value, If there is no value for the key strKey
    bool GetBool(const std::string& strKey, bool default_value = false) const;
    float64 GetDouble(const std::string& strKey, float64 default_value = 0.0) const;
    int64 GetInteger(const std::string& strKey, int64 default_value = 0) const;
    const std::string& GetString(const std::string& strKey, const std::string& default_value = StringUtil::kEmpty) const;

    // Get a decimal number whether it is a JSONDouble or a JSONInteger
    simcc::float64 GetDecimal(const std::string& strKey, float64 default_value = 0.0) const;

    //   Put a key/value pair into the JSONObject.
    //   If a item which is associated with key is exist,
    // the old value is deleted, and the new value is inserted.
    //   If the value is null, then the key will be removed
    // from the JSONObject if it is present.
    // @param key   A key string.
    // @param value An object which is the value.
    // @return true, if successfully.
    //       false, if failed.
    bool Put(const std::string& key, Object* value);//Do not delete this pointer, it will be managed by the JSONObject
    bool Put(const std::string& key, const ObjectPtr& value);
    bool Put(const std::string& key, const int64 value);
    bool Put(const std::string& key, const float64 value);
    bool Put(const std::string& key, const bool value);
    bool Put(const std::string& key, const std::string& value);
    bool Put(const std::string& key, const char* value);

    // Put an array to this JSONObject
    // @param key, the key
    // @param value the value array
    // @param count Size of array
    // @return true, if successfully
    //   or false, if failed
    bool PutByteArray(const std::string& key, const simcc::int8* value, simcc::uint32 count);
    bool PutInt32Array(const std::string& key, const simcc::int32* value, simcc::uint32 count);
    bool PutInt64Array(const std::string& key, const simcc::int64* value, simcc::uint32 count);
    bool PutFloat32Array(const std::string& key, const simcc::float32* value, simcc::uint32 count);
    bool PutFloat64Array(const std::string& key, const simcc::float64* value, simcc::uint32 count);
    bool PutBoolArray(const std::string& key, const bool* value, simcc::uint32 count);
    bool PutStringArray(const std::string& key, const std::string* value, simcc::uint32 count);

    // Remove a key/value pair from this JSONObject.
    void Remove(const std::string& key) { erase(key); }
    void Erase(const std::string& key) { erase(key); }
    void Remove(const Object* value);

    void erase(iterator it) {
        map_.erase(it);
    }

    void erase(const std::string& key) {
        map_.erase(key);
    }

    // Returns the number of elements in the this JSON object
    size_t size() const {
        return map_.size();
    }

    // Returns whether the JSON object is empty, i.e. whether its size is 0.
    bool empty() const {
        return map_.empty();
    }

    // All the elements in the list container are dropped: their destructors are called,
    // and then they are removed from the list container, leaving it with a size of 0.
    void clear() {
        map_.clear();
    }

    // Merge another JSONObject to this JSONObject
    // For example:
    // A : { "keyA1" : "valueA1", "keyA2" : "valueA2", "same-key" : "old value" }
    // B : { "keyB1" : "valueB", "same-key" : "new value from B" }
    // A.Merge(B, true) ==> A will change to : { "keyA1" : "valueA1", "keyA2" : "valueA2", "keyB1" : "valueB", "same-key" : "new value from B" },
    //                and B don't change anything.
    // @note If <code>rhs</code> has some same-key as this JSONObject,
    //   this new value from <code>rhs</code> will override the original value
    //   when <code>override</code> is true; or the value of the key in this
    //   JSONObject will has no change.
    // @param rhs
    // @param override -
    void Merge(const JSONObject* rhs, bool override);

    // iterator
public:
    // Gets object map container.
    const ObjectPtrMap& GetObjects() const {
        return map_;
    }

    // Gets object map container.
    ObjectPtrMap& GetObjects() {
        return map_;
    }

    const_iterator begin() const {
        return map_.begin();
    }

    iterator begin() {
        return map_.begin();
    }

    const_iterator end() const {
        return map_.end();
    }

    iterator end() {
        return map_.end();
    }

    const_reverse_iterator rbegin() const {
        return map_.rbegin();
    }

    reverse_iterator rbegin() {
        return map_.rbegin();
    }

    const_reverse_iterator rend() const {
        return map_.rend();
    }

    reverse_iterator rend() {
        return map_.rend();
    }

protected:
    // Produce a string in simcc::f64 quotes with backslash sequences in all the
    // right places. A backslash will be inserted within </, allowing JSON
    // text to be delivered in HTML. In JSON text, a string cannot contain a
    // control character or an unescaped quote or backslash.
    // @param source  A the source String
    // @param rs the produced string by this function
    // @return  true, if success, or false
    static void Quote(const std::string& source, bool utf8_to_unicode, simcc::DataStream& sb);

    template<class T>
    bool PutIntegerArray(const std::string& key, const T* value, simcc::uint32 count);

    friend class JSONTokener;
    friend class JSONArray;
    friend class JSONString;
    friend class JSONDouble;
    friend class JSONInteger;
    friend class JSONBoolean;
    friend class JSONNull;

    // Return number of characters parsed.
    simcc::uint32 Parse(JSONTokener* token);

    JSONObject(JSONTokener* token);

private:
    friend class JSONParser;

    // override method from base class json::Object
    virtual bool LoadFrom(simcc::DataStream& file);
    virtual void SaveTo(simcc::DataStream& file) const;

private:
    ObjectPtrMap map_;
};

typedef simcc::RefPtr<JSONObject>  JSONObjectPtr;
typedef simcc::RefPtr<JSONInteger> JSONIntegerPtr;
typedef simcc::RefPtr<JSONDouble>  JSONDoublePtr;
typedef simcc::RefPtr<JSONArray>   JSONArrayPtr;
typedef simcc::RefPtr<JSONString>  JSONStringPtr;
typedef simcc::RefPtr<JSONBoolean> JSONBooleanPtr;
typedef simcc::RefPtr<JSONNull>    JSONNullPtr;
typedef simcc::RefPtr<Object>      ObjectPtr;

}
}
