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
 > FilePath: /zerone216/zslib/tools/zs_json_utils.h
 > CreateTime: 2019-12-15 18:51:27
 > ModifyTime: 2020-01-03 22:28:05
 > LastEditors: 
 > Description: 
 > version: 1.0.0
 > Repository: https://github.com/zerone216/
 > Support: 
 > Others: 
 > Changelogs: 
 */
#pragma once

#include "core/zs_stdc++.h"

namespace zerone {
/**
 * @brief json数据类型
 */
enum class zs_json_type {
    null = 0,
    boolean = 1,
    number = 2,
    string = 3,
    array = 4,
    object = 5,
};

/**
 * json 值结构
 */
struct zs_json_value final {
    zs_json_type type;                                          // 类型
    std::string value;                                          // 值
    std::vector<std::shared_ptr<zs_json_value>> array_value;    // 数组类型
    std::unordered_map<std::string, std::shared_ptr<zs_json_value>>
        object_value;    // 对象类型
};
}    // namespace zerone