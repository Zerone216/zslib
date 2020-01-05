/*
 > Declaration: 
 >  Copyright (c) 2009-2019 Zhang Sheng <zerone216@163.com>
 >  See my homepage: https://zerone216.cn
 > 
 >  This program is free software; you can redistribute it and/or modify it under 
 >  the terms of the GNU General Public License as published by the Free Software 
 >  Foundation, either version 2 or any later version.
 >  
 >  Redistribution and use in source and binary forms, with or without modification,
 >  are permitted provided that the following conditions are met: 
 >  
 >  1. Redistributions of source code must retain the above copyright notice, this
 >  list of conditions and the following disclaimer.
 >  
 >  2. Redistributions in binary form must reproduce the above copyright notice,
 >  this list of conditions and the following disclaimer in the documentation and/or 
 >  other materials provided with the distribution. 
 > 
 >  This program is distributed in the hope that it will be useful, but WITHOUT ANY 
 >  WARRANTY; without even the implied warranty of MERCHANTABILITY or ITNESS FOR A 
 >  PARTICULAR PURPOSE. See the GNU General Public License for more details. A copy 
 >  of the GNU General Public License is available at: 
 >  http://www.fsf.org/licensing/licenses 
 >  
 >  All manufacturing, reproduction, use, and sales rights pertaining to this subject 
 >  matter are governed bythe license agreement. The recipient of this software 
 >  implicitly accepts the terms of the license.
 > 
 > Author: Zhang Sheng
 > FilePath: /zerone216/zslib/tools/zs_object_factory.hpp
 > CreateTime: 2020-01-03 21:03:24
 > ModifyTime: 2020-01-05 21:55:38
 > LastEditors: 
 > Description: 
 > version: 
 > Repository: https://github.com/zerone216/
 > Support: 
 > Others: 
 > Changelogs: 
 */

#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#include "core/zs_stdc++.h"
#include "zs_object_factory.h"
#include "zs_utils.hpp"
#include "tools/zs_json.hpp"

#include "tools/zs_logger.hpp"

namespace zerone {
bool zs_object_factory::load_config(const std::string &config_str) {
    auto config_obj = zs_json::from_string(config_str);
    if (config_obj.is_null()) {
        return false;
    }
    return load_data__(config_obj);
}

bool zs_object_factory::load_config_file(const std::string &config_file) {
    byte_array data;
    if (zs_read_file(config_file, data) == false) {
        return false;
    }
    return load_config(zerone::to_string(data));
}

bool zs_object_factory::set_config(const zs_json &config_obj) {
    return load_data__(config_obj);
}

bool zs_object_factory::load_data__(const zs_json &config_obj) {
    object_data__.clear();
    auto size = config_obj.size();
    for (int i = 0; i < size; ++i) {
        zs_object_factory_config_item_t item;
        auto tmp_obj = config_obj.at(i);
        item.id = static_cast<std::string>(tmp_obj["id"]);
        item.data = tmp_obj["data"];
        item.singleton = tmp_obj["singleton"];
        if (item.id.empty()) {
            object_data__.clear();
            return false;
        }
        object_data__[item.id] = item;
    }
    return true;
}

zs_json zs_object_factory::object_data(const std::string &obj_name) {
    if (object_data__.count(obj_name) == 0) {
        return zs_json();
    }
    auto obj = object_data__[obj_name].data.deep_copy();
    if (obj.has("__base__")) {
        auto base_obj = object_data(static_cast<std::string>(obj["__base__"]));
        obj.remove("__base__");
        base_obj.join(obj);
        return base_obj;
    } else {
        return obj;
    }
}

template <typename T, typename... ARGS>
std::shared_ptr<T> zs_object_factory::object(const std::string &obj_id,
                                             ARGS &&... args) {
    if (object_data__.count(obj_id) == 0) {
        return nullptr;
    }
    auto &tmp_config = object_data__[obj_id];

    if (tmp_config.singleton) {
        if (tmp_config.object.has_value()) {
            return std::any_cast<std::shared_ptr<T>>(tmp_config.object);
        }
    }
    std::shared_ptr<T> obj;
    obj = std::make_shared<T>(std::forward<ARGS>(args)...);

    auto obj_data = object_data(obj_id);

    from_json(obj_data, *obj);
    tmp_config.object = obj;
    return obj;
}
}    // namespace zerone
#pragma clang diagnostic pop