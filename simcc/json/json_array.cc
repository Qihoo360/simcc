#include "simcc/inner_pre.h"

#include "json.h"
#include "json_tokener.h"

namespace simcc {
namespace json {

JSONArray::JSONArray(JSONTokener* token)
    : Object(kJSONArray) {
    Parse(token);
}

JSONArray::JSONArray(const string& source)
    : Object(kJSONArray) {
    Parse(source);
}

JSONArray::JSONArray(const char* source)
    : Object(kJSONArray) {
    Parse(source);
}

JSONArray::~JSONArray() {
}

simcc::uint32 JSONArray::Parse(const string& source) {
    JSONTokener x(source);
    return Parse(&x);
}

simcc::uint32 JSONArray::Parse(const char* source, const simcc::int64 len) {
    if (len == 0 || !source) {
        set_error(kParameterWrong);
        return 0;
    }

    JSONTokener x(source, len);
    return Parse(&x);
}

simcc::uint32 JSONArray::Parse(JSONTokener* x) {
    if (!x->SkipComment()) {
        set_error(kCommentFormatError, x);
        return 0;
    }

    char c = x->NextClean();
    char q = 0;
    if (c == '[') {
        q = ']';
    } else if (c == '(') {
        q = ')';
    } else {
        set_error(kJSONArrayNotBeginWithBrackets, x);
        return 0;
    }

    if (!x->SkipComment()) {
        set_error(kCommentFormatError, x);
        return 0;
    }

    c = x->NextClean();
    if (c == q) {
        return x->GetCurrentPosition();
    }

    x->Back();
    Object* jo = NULL;
    for (;;) {
        if (!x->SkipComment()) {
            set_error(kCommentFormatError, x);
            return 0;
        }

        c = x->NextClean();
        if (c == ',') {
            x->Back();
            list_.push_back(new JSONNull());
        } else {
            x->Back();
            jo = x->NextValue(this);
            if (jo) {
                list_.push_back(jo);
            } else {
                return 0;
            }
        }

        if (!x->SkipComment()) {
            set_error(kCommentFormatError, x);
            return 0;
        }

        c = x->NextClean();

        switch (c) {
        case ';':
        case ',':
            if (!x->SkipComment()) {
                set_error(kCommentFormatError, x);
                return 0;
            }

            c = x->NextClean();
            if (c == q) {
                return x->GetCurrentPosition();
            }

            x->Back();
            break;
        case ']':
        case ')':
            if (q != c) {
                set_error(kJSONArrayNotEndWithBrackets, x);
                return 0;
            }
            return x->GetCurrentPosition();
        default:
            set_error(kJSONArrayNotEndWithBrackets, x);
            return 0;
        }
    }
}

void JSONArray::ToStringBuf(simcc::DataStream& sb, size_t indent, bool utf8_to_unicode)const {
    static const char* indentstr =
        "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t"
        "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t"
        "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t"
        "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t"
        "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t"
        "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";
    if (indent > 1) {
        assert(indent < strlen(indentstr));
        sb.Write(indentstr, indent - 1);
    }

    ObjectPtrList::const_iterator it(list_.begin()), ite(list_.end());
    sb.Write('[');

    if (indent > 0) {
        sb.Write('\n');
    }

    string temp;
    bool need_comma = false;
    for (; it != ite; it++) {
        if (need_comma) {
            sb.Write(',');
            if (indent > 0) {
                sb.Write('\n');
            }
        } else {
            need_comma = true;
        }

        if (indent > 0) {
            (*it)->ToStringBuf(sb, indent + 1, utf8_to_unicode);
        } else {
            (*it)->ToStringBuf(sb, 0, utf8_to_unicode);
        }
    }

    // Write end close ]
    if (indent > 0) {
        sb.Write('\n');
        sb.Write(indentstr, indent - 1);
    }
    sb.Write(']');
}

void JSONArray::ToString(string& s, bool readable, bool utf8_to_unicode)const {
    buf_.Reset();
    size_t indent = 0;

    if (readable) {
        indent = 1;
    }

    ToStringBuf(buf_, indent, utf8_to_unicode);
    s.assign(buf_.data(), buf_.size());
}

bool JSONArray::Equals(const Object& rhs) {
    if (rhs.type() != type()) {
        return false;
    }

    const JSONArray& rhsArray = dynamic_cast<const JSONArray&>(rhs);
    if (rhsArray.size() != size()) {
        return false;
    }

    const_iterator itthis(begin());
    const_iterator itethis(end());
    const_iterator itrhs(rhsArray.begin());
    const_iterator iterhs(rhsArray.end());
    for (; itthis != itethis && itrhs != iterhs; ++itthis, ++itrhs) {
        if (!(*itthis)->Equals(*(*itrhs))) {
            return false;
        }
    }

    return true;
}


bool JSONArray::Remove(int index) {
    iterator it = begin(index);
    if (it == list_.end()) {
        return false;
    }

    list_.erase(it);
    return true;
}

Object* JSONArray::Get(int index) const {
    const_iterator it = begin(index);
    if (it != list_.end()) {
        return (*it);
    }

    return NULL;
}

template<class T>
T* JSONArray::GetObject(int index) const {
    Object* o = Get(index);
    return cast<T>(o);
}


JSONBoolean* JSONArray::GetJSONBoolean(int index) const {
    return GetObject<JSONBoolean>(index);
}

JSONDouble* JSONArray::GetJSONDouble(int index) const {
    return GetObject<JSONDouble>(index);
}

JSONInteger* JSONArray::GetJSONInteger(int index) const {
    return GetObject<JSONInteger>(index);
}

JSONArray* JSONArray::GetJSONArray(int index) const {
    return GetObject<JSONArray>(index);
}

JSONObject* JSONArray::GetJSONObject(int index) const {
    return GetObject<JSONObject>(index);
}

JSONString* JSONArray::GetJSONString(int index) const {
    return GetObject<JSONString>(index);
}


bool JSONArray::IsNull(int index) const {
    const_iterator it = begin(index);
    if (it == list_.end()) {
        return true;
    }

    return cast<JSONNull>((*it).get()) ? true : false;
}

JSONArray* JSONArray::Put(const bool value) {
    list_.push_back(new JSONBoolean(value));
    return this;
}

JSONArray* JSONArray::Put(const simcc::float64 value) {
    list_.push_back(new JSONDouble(value));
    return this;
}

JSONArray* JSONArray::Put(const simcc::int64 value) {
    list_.push_back(new JSONInteger(value));
    return this;
}

JSONArray* JSONArray::Put(const char* value) {
    list_.push_back(new JSONString(value));
    return this;
}

JSONArray* JSONArray::Put(const string& value) {
    return Put(value.c_str());
}

JSONArray* JSONArray::Put(Object* value) {
    if (value) {
        list_.push_back(value);
    }

    return this;
}

JSONArray* JSONArray::Put(const ObjectPtr& value) {
    return Put(value.get());
}

template<class T>
const T& JSONArray::GetElement(int index , const T& default_value)const {
    typedef typename ToJSONType<T>::JSONClass JSONClass;

    Object* o = Get(index);
    if (o) {
        JSONClass* p = cast<JSONClass>(o);
        if (p) {
            return p->value();
        }
    }

    return default_value;
}

bool JSONArray::GetBool(int index, bool default_value)const {
    return GetElement(index , default_value);
}

float64 JSONArray::GetDouble(int index, float64 default_value)const {
    return GetElement(index , default_value);
}

int64 JSONArray::GetInteger(int index, int64 default_value)const {
    return GetElement(index , default_value);
}

const string& JSONArray::GetString(int index, const string& default_value)const {
    return GetElement(index , default_value);
}

simcc::float64 JSONArray::GetDecimal(int index, float64 default_value) const {
    Object* o = Get(index);
    if (o) {
        if (o->IsTypeOf(kJSONDouble)) {
            return ((JSONDouble*)o)->value();
        }

        if (o->IsTypeOf(kJSONInteger)) {
            return ((JSONInteger*)o)->value();
        }
    }
    return default_value;
}

template<class T, class U>
void JSONArray::GetElement(T* array , uint32 count , const U& default_value)const {
    for (simcc::uint32 i = 0; i < count; ++ i) {
        array[i] = static_cast<T>(GetElement(static_cast<int>(i), default_value));
    }
}

void JSONArray::GetBoolArray(bool* array, simcc::uint32 count, bool default_value)const {
    GetElement(array , count , default_value);
}

void JSONArray::GetByteArray(simcc::uint8* array, simcc::uint32 count, simcc::uint8 default_value)const {
    GetElement(array , count , static_cast<simcc::int64>(default_value));
}

void JSONArray::GetInt32Array(simcc::int32* array, simcc::uint32 count, simcc::int32 default_value)const {
    GetElement(array , count , static_cast<simcc::int64>(default_value));
}

void JSONArray::GetInt64Array(simcc::int64* array, simcc::uint32 count, simcc::int64 default_value)const {
    GetElement(array , count , static_cast<simcc::int64>(default_value));
}

void JSONArray::GetFloat32Array(simcc::float32* array, simcc::uint32 count, simcc::float32 default_value)const {
    GetElement(array , count , static_cast<simcc::float64>(default_value));
}

void JSONArray::GetFloat64Array(simcc::float64* array, simcc::uint32 count, simcc::float64 default_value)const {
    GetElement(array , count , static_cast<simcc::float64>(default_value));
}

void JSONArray::GetStringArray(string* array, simcc::uint32 count, const string& default_value)const {
    GetElement(array , count , default_value);
}

template<class T, class U>
void JSONArray::GetElement(std::vector<T>& vArray, const U& default_value)const {
    size_t count = list_.size();
    vArray.resize(count);
    T* array = &vArray[0];
    GetElement(array , count , default_value);
}

void JSONArray::GetBoolArray(std::vector<bool>& vArray, bool default_value)const {
    //由于vector<bool>是个特化版本,所以要特别处理一下.
    size_t count = list_.size();
    vArray.resize(count);

    auto array = new bool[count];
    GetElement(array , count , default_value);
    std::copy(array , array + count , vArray.begin());
    delete[] array;
}

void JSONArray::GetByteArray(std::vector<simcc::uint8>& vArray, simcc::uint8 default_value)const {
    GetElement(vArray , static_cast<simcc::int64>(default_value));
}

void JSONArray::GetInt32Array(std::vector<simcc::int32>& vArray, simcc::int32 default_value)const {
    GetElement(vArray , static_cast<simcc::int64>(default_value));
}

void JSONArray::GetInt64Array(std::vector<simcc::int64>& vArray, simcc::int64 default_value)const {
    GetElement(vArray , static_cast<simcc::int64>(default_value));
}

void JSONArray::GetFloat32Array(std::vector<simcc::float32>& vArray, simcc::float32 default_value)const {
    GetElement(vArray , static_cast<simcc::float64>(default_value));
}

void JSONArray::GetFloat64Array(std::vector<simcc::float64>& vArray, simcc::float64 default_value)const {
    GetElement(vArray , static_cast<simcc::float64>(default_value));
}

void JSONArray::GetStringArray(std::vector<string>& vArray, const string& default_value)const {
    GetElement(vArray , default_value);
}

JSONArray::const_iterator JSONArray::begin(size_t index) const {
    if (index > list_.size()) {
        return list_.end();
    }

    const_iterator it = list_.begin();
    std::advance(it , index);
    return it;
}

JSONArray::iterator JSONArray::begin(size_t index) {
    if (index > list_.size()) {
        return list_.end();
    }

    iterator it = list_.begin();
    std::advance(it , index);  //        for ( size_t i = 0; i < index; i++ ) it++;
    return it;
}

void JSONArray::SaveTo(simcc::DataStream& file) const {
    simcc::uint32 nSize = (simcc::uint32)this->size();
    file << (simcc::uint8)type()        //type
         << (simcc::uint32)nSize;                //size

    for (simcc::uint32 i = 0; i < nSize; i++) {
        Get(i)->SaveTo(file);
    }
}

bool JSONArray::LoadFrom(simcc::DataStream& file) {
    simcc::uint32 nSize = 0;
    file >> nSize;

    for (simcc::uint32 i = 0; i < nSize; i++) {
        Object* o = NULL;
        if (Object::DeserializeOneObject(file, o)) {
            assert(o);
            Put(o);
            o = NULL;
        } else {
            if (o) {
                delete o;
            }
            return false;
        }
    }

    return true;
}

}
}
