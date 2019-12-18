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
 > FilePath: /github/zerone216/zslib/core/zs_type.hpp
 > CreateTime: 2019-12-18 20:32:49
 > ModifyTime: 2019-12-18 21:57:03
 > LastEditors: 
 > Description: 
 > version: 
 > Repository: https://github.com/zerone216/
 > Support: 
 > Others: 
 > Changelogs: 
 */

/*
 * 常用类型声明
 */

#pragma once

#include "zs_type.h"

namespace zerone {

inline std::string to_string(const byte_array &data) {
    return std::string(data.begin(), data.end());
}

inline byte_array operator+(byte_array b1, const byte_array &b2) {
    b1.insert(b1.end(), b2.begin(), b2.end());
    return b1;
}

inline byte_array &operator+=(byte_array &b1, const byte_array &b2) {
    b1.insert(b1.end(), b2.begin(), b2.end());
    return b1;
}

template <typename T>
inline byte_array to_byte_array(const T &t) {
    byte_array ret(sizeof(t));
    std::memcpy(ret.data(), &t, sizeof(t));
    return ret;
}

inline byte_array to_byte_array(const std::string &str) {
    byte_array ret(str.length());
    std::memcpy(ret.data(), str.data(), str.length());
    return ret;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
inline bool dump_byte_array(const byte_array &data, const std::string &filename,
                            bool app) {
    std::ofstream fo(filename, app ? (std::ios::app | std::ios::binary)
                                   : (std::ios::trunc | std::ios::out));
    if (!fo) return false;
    fo.write(data.data(), data.size());
    return true;
}
#pragma clang diagnostic pop

}    // namespace zerone
