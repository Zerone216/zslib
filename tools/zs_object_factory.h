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
 > FilePath: /zerone216/zslib/tools/zs_object_factory.h
 > CreateTime: 2020-01-03 20:45:19
 > ModifyTime: 2020-01-05 21:54:40
 > LastEditors: 
 > Description: 
 > version: 1.0.0
 > Repository: https://github.com/zerone216/
 > Support: 
 > Others: 
 > Changelogs: 
 */

#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#include "core/zs_stdc++.h"
#include "zs_object_factory_utils.h"
#include "tools/zs_json.h"
#include "tools/zs_nocopy.h"

namespace zerone {
class zs_object_factory final : public zs_nocopy<> {
   public:
    /**
     * @brief 加载配置
     *
     * @param config_str 配置字符串
     * @return true 加载成功
     * @return false 加载失败
     */
    bool load_config(const std::string& config_str);
    /**
     * @brief 加载配置文件
     *
     * @param config_file 配置文件
     * @return true 加载成功
     * @return false 加载失败
     */
    bool load_config_file(const std::string& config_file);
    /**
     * @brief Set the config object设置配置
     *
     * @param config_obj json配置
     * @return true 设置成功
     * @return false 设置失败
     */
    bool set_config(const zs_json& config_obj);

    /**
     * @brief 获取对象
     *
     * @tparam T 类型
     * @tparam ARGS 构造函数参数类型
     * @param obj_id id
     * @param args 参数
     * @return std::shared_ptr<T> 对象
     */
    template <typename T, typename... ARGS>
    std::shared_ptr<T> object(const std::string& obj_id, ARGS&&... args);

   private:
    std::unordered_map<std::string, zs_object_factory_config_item_t>
        object_data__;

    bool load_data__(const zs_json& config_obj);
    zs_json object_data(const std::string& obj_name);
};
}    // namespace zerone
#pragma clang diagnostic pop