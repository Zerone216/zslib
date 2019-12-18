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
 > FilePath: /github/zerone216/zslib/core/zs_type.h
 > CreateTime: 2019-12-18 20:15:06
 > ModifyTime: 2019-12-18 21:55:11
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

#include <fstream>
#include <string>
#include <vector>

#ifndef _WIN32
#define SOCKET int
#endif

#define ZS_GET_OFFSET(obj, member) \
    (reinterpret_cast<size_t>(&(reinterpret_cast<obj *>(0)->member)))

#include <cstring>
#include <iostream>

namespace zerone {
/**
 *  @brief 字节数组类型
 */
using byte_array = std::vector<char>;

/**
 * 字节数组转字符串
 * @param data 字节数组
 * @return 字符串
 */
inline std::string to_string(const byte_array &data);

/**
 * 合并字节数组
 * @param b1 字节数组1
 * @param b2 字节数组2
 * @return 字节数组合并
 */
inline byte_array operator+(byte_array b1, const byte_array &b2);

/**
 * 追加字节数组
 * @param b1 字节数组1
 * @param b2 字节数组2
 * @return 新的字节数组
 */
inline byte_array &operator+=(byte_array &b1, const byte_array &b2);

/**
 * pod类型转换为字节数组
 * @tparam T 类型
 * @param t 数据
 * @return 字节数组
 */
template <typename T>
inline byte_array to_byte_array(const T &t);

/**
 * 字符串转换为字节数组
 * @param str 字符串
 * @return 字节数组
 */
inline byte_array to_byte_array(const std::string &str);

/**
 * 保存字节数组到文件
 * @param data 数据
 * @param filename 文件名称
 * @param app 是否是追加
 * @return 是否dump成功
 */
inline bool dump_byte_array(const byte_array &data, const std::string &filename,
                            bool app = false);
}    // namespace zerone
