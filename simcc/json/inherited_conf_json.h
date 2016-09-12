/********************************************************************
    created:    2014/06/19
    created:    19:6:2014   11:10
    filename:   simcc/inherited_json.h
    author:     weizili

    purpose:
*********************************************************************/

#pragma once

#include "json_common.h"
#include "json_value.h"
#include "json_array.h"
#include "json_object.h"

namespace simcc {
namespace json {
class SIMCC_EXPORT InheritedConfJSONObject {
public:
    // @brief 从一个json文件中解析出json对象。
    //     这个json文件一般是一个配置文件。该配置文件中如果有"inherited_from"这个key，
    // 表明该配置文件的某些配置项是继承自inherited_from指向的文件，我们称之为默认配置。
    // 当前这个配置文件中的配置项会覆盖默认配置中同名key。
    // @param[in] - const std::string & json_file_path
    // @return - json::JSONObjectPtr
    static json::JSONObjectPtr Parse(const std::string& json_file_path);
};

}
}