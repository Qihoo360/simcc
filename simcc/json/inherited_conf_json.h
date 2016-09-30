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
    // @brief 从JSON格式的文件中解析出JSON对象。
    //     这个JSON文件一般是一个配置文件。该配置文件中如果有 "inherited_from" 这个key，
    // 表明该配置文件的某些配置项是继承自 "inherited_from" 指向的文件，我们称之为默认配置。
    // 当前这个配置文件中的配置项会覆盖默认配置中同名key。
    // @param[in] - const string& json_file_path
    // @return - json::JSONObjectPtr 如果失败会返回一个空对象
    static json::JSONObjectPtr Parse(const string& json_file_path);
};

}
}