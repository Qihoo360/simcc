// Created: 2009/08/07
// Author: weizili
#pragma once

#include "simcc/inner_pre.h"

#include "json_common.h"

namespace simcc {
namespace json {

template<typename T >
T* cast(json::Object* obj) {
    if (obj) {
        if (static_cast<JSONType>(T::Type) == obj->type()) {
            return static_cast<T*>(obj);
        }
    }
    return NULL;
}

template<typename T >
const T* cast(const json::Object* obj) {
    if (obj) {
        if (static_cast<JSONType>(T::Type) == obj->type()) {
            return static_cast<const T*>(obj);
        }
    }
    return NULL;
}

template<typename T >
T* cast(json::ObjectPtr& obj) {
    if (obj) {
        if (static_cast<JSONType>(T::Type) == obj->type()) {
            return static_cast<T*>(obj.get());
        }
    }
    return NULL;
}

template<typename T >
const T* cast(const json::ObjectPtr& obj) {
    if (obj) {
        if (static_cast<JSONType>(T::Type) == obj->type()) {
            return static_cast<const T*>(obj.get());
        }
    }
    return NULL;
}

}
}

