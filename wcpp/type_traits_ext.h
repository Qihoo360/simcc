#pragma once

#include "wcpp/platform_config.h"
#include <type_traits>

#define H_IS_POD_TYPE( type , booltype ) namespace std {    \
    template<>                                              \
    struct is_pod<type> : public booltype##_type {};        \
    }

