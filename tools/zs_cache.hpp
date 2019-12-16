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
 >  http://www.fsf.org/licensing/licenses 
 >  
 >  All manufacturing, reproduction, use, and sales rights pertaining to this subject 
 >  matter are governed bythe license agreement. The recipient of this software 
 >  implicitly accepts the terms of the license.
 > 
 > Author: Zhang Sheng
 > FilePath: /github/zerone216/zslib/tools/zs_cache.hpp
 > CreateTime: 2019-12-14 12:21:23
 > ModifyTime: 2019-12-16 22:51:32
 > LastEditors: 
 > Description: 
 > version: 
 > Repository: https://github.com/zerone216/
 > Support: 
 > Others: 
 > Changelogs: 
 */

#pragma once

#include "zs_cache.h"
#include "tools/zs_nocopy.h"
#include "tools/zs_utils.hpp"

namespace zerone {
inline zs_cache::zs_cache(int max_count)
    : max_count__(max_count)
{
    data__.reserve(max_count__);
}

template <typename T>
std::shared_ptr<T> zs_cache::data(const std::string& key)
{
    std::lock_guard<std::recursive_mutex> lck(mu_data__);
    for (auto& p : data__) {
        if (p.key == key) {
            p.timestamp_access = std::chrono::system_clock::now();
            return std::make_shared<T>(std::any_cast<T>(p.data));
        }
    }
    return nullptr;
}

template <typename T>
void zs_cache::set_data(const std::string& key, const T& d)
{
    cache_data_t tmp { key, std::chrono::system_clock::now(), d };
    std::lock_guard<std::recursive_mutex> lck(mu_data__);
    if (data__.size() < max_count__) {
        data__.push_back(tmp);
        return;
    }
    auto min_tm = data__[0].timestamp_access;
    auto min_index = 0;
    for (auto i = 1UL; i < data__.size(); ++i) {
        if (data__[i].key == key) {
            data__[i] = tmp;
            return;
        }
        if (min_tm > data__[i].timestamp_access) {
            min_tm = data__[i].timestamp_access;
            min_index = i;
        }
    }
    data__[min_index] = tmp;
}
} // namespace zerone
