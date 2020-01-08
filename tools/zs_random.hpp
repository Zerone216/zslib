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
 > FilePath: /zerone216/zslib/tools/zs_random.hpp
 > CreateTime: 2020-01-07 20:15:28
 > ModifyTime: 2020-01-08 22:45:49
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

#include "core/zs_msvc_safe.h"
#include "zs_random.h"

namespace zerone {
inline zs_random::zs_random() {
    e__ = std::make_shared<std::default_random_engine>(rd__());
}

inline int zs_random::rand_int(const int min, const int max) const {
    std::uniform_int_distribution<int> ed(min, max);
    return ed(*e__.get());
}

inline double zs_random::rand_double(const double min, const double max) const {
    std::uniform_real_distribution<double> ed(min, max);
    return ed(*e__.get());
}

inline std::string zs_random::uuid_str() const {
    std::uniform_int_distribution<int> ed(INT_MIN, INT_MAX);
    int data[4]{ed(*e__.get()), ed(*e__.get()), ed(*e__.get()), ed(*e__.get())};
    char c_str[40];
    zs_safe_sprintf(c_str, 40, "%0X%0X%0X%0X", data[0], data[1], data[2],
                    data[3]);
    std::string ret = c_str;
    ret.insert(ret.begin() + 20, '-');
    ret.insert(ret.begin() + 16, '-');
    ret.insert(ret.begin() + 12, '-');
    ret.insert(ret.begin() + 8, '-');
    return ret;
}
}    // namespace zerone
#pragma clang diagnostic pop