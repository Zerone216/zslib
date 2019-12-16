/*
 > Declaration: 
 >  Copyright (c) 2009-2019 Zhang Sheng <zerone216@163.com>
 >  See my homepage: http://www.zerone216.top:8080
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
 >  http://www.fzs.org/licensing/licenses 
 >  
 >  All manufacturing, reproduction, use, and sales rights pertaining to this subject 
 >  matter are governed bythe license agreement. The recipient of this software 
 >  implicitly accepts the terms of the license.
 > 
 > Author: Zhang Sheng
 > FilePath: /github/zerone216/zslib/tools/zs_cache.h
 > CreateTime: 2019-12-14 12:23:26
 > ModifyTime: 2019-12-16 22:49:36
 > LastEditors: 
 > Description: 
 > version: 
 > Repository: https://github.com/zerone216/
 > Support: 
 > Others: 
 > Changelogs: 
 */

#pragma once

#include "core/zs_define.h"
#include "core/zs_stdc++.h"
#include "tools/zs_nocopy.h"
#include "tools/zs_utils.h"

namespace zerone {
/**
 * @brief 内存缓存
 *
 */
class zs_cache
    : public zs_make_instance_t<zs_cache, zs_nocopy<zs_empty_class>> {
private:
    struct cache_data_t {
        std::string key;
        std::chrono::system_clock::time_point timestamp_access;
        std::any data;
    };
    std::vector<cache_data_t> data__;
    size_t max_count__;
    std::recursive_mutex mu_data__;

public:
    /**
     * @brief Construct a new zs cache object
     *
     * @param max_count 缓存条目数量
     */
    explicit zs_cache(int max_count = default_cache_max_count);
    /**
     * @brief 取得缓存数据
     *
     * @tparam T 数据类型（如果数据类型不符合会抛出bad_cast异常）
     * @param key 键
     * @return std::shared_ptr<T> 数据
     */
    template <typename T>
    std::shared_ptr<T> data(const std::string& key);
    /**
     * @brief 设置缓存
     *
     * @tparam T 类型
     * @param key 键
     * @param d 值
     */
    template <typename T>
    void set_data(const std::string& key, const T& d);
};
} // namespace zerone
