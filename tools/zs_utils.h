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
 > FilePath: /github/zerone216/zslib/tools/zs_utils.h
 > CreateTime: 2019-12-18 21:29:11
 > ModifyTime: 2019-12-18 22:45:52
 > LastEditors: 
 > Description: 
 > version: 
 > Repository: https://github.com/zerone216/
 > Support: 
 > Others: 
 > Changelogs: 
 */

#pragma once

#include <sys/stat.h>
#include <sys/types.h>
#include "core/zs_stdc++.h"
#include "core/zs_empty_class.h"
#include "core/zs_type.hpp"

namespace zerone {
/**
 * 字符串去除首尾空白
 * @param str 字符串
 * @return 处理后的字符串
 */
inline std::string zs_string_trim(const std::string &str);

/**
 * 字符串转小写
 * @param str 字符串
 * @return 处理后的字符串
 */
inline std::string zs_to_lower_string(std::string str);

/**
 * 获取文件后缀
 * @param path 路径
 * @return 后缀
 */
inline std::string zs_get_path_ext(const std::string &path);

/**
 * 不区分大小写比较字符串
 * @param str1 字符串1
 * @param str2 字符串2
 * @return 是否相等
 */
inline bool zs_equal_nocase_string(const std::string &str1,
                                   const std::string &str2);

/**
 * 分割字符串
 * @param str 原始字符串
 * @param split_str 分隔符
 * @return 分割后的字符串数组
 */
inline std::vector<std::string> zs_split_string(std::string str,
                                                const std::string &split_str);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

/**
 * 写文件
 * @param file_name 文件名称
 * @param data 数据
 * @param append 是否追加
 * @return 是否写入成功
 */
bool zs_write_file(const std::string &file_name, const byte_array &data,
                   bool append);

#pragma clang diagnostic pop

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

/**
 * 读文件
 * @param file_name 文件名称
 * @param data 数据
 * @return 是否读取成功
 */
bool zs_read_file(const std::string &file_name, byte_array &data);

#pragma clang diagnostic pop

/**
 * 字符串替换
 * @param str 字符串
 * @param from 要替换的字符串
 * @param to 替换后的新字符串
 */
void zs_string_replace(std::string &str, const std::string &from,
                       const std::string &to);

/**
 * 判断字符串是否是由指定前缀开始的
 * @param str 字符串
 * @param prefix 前缀
 * @return 字符串是否是由指定前缀开始的
 */
bool zs_string_start_with(const std::string &str, const std::string &prefix);

/**
 * 判断字符串是否是由指定后缀结束
 * @param str 字符串
 * @param suffix 后缀
 * @return 字符串是否是由指定后缀结束
 */
bool zs_string_end_with(const std::string &str, const std::string &suffix);

/**
 * 生成时间字符串
 * @return 时间字符串
 */
std::string zs_make_time_str(const std::chrono::system_clock::time_point &tp =
                                 std::chrono::system_clock::now() +
                                 std::chrono::hours(8));

/**
 * long double 转为字符串
 * @param num 数字
 * @return 字符串
 */
std::string zs_long_double_to_string(const long double &num);

/**
 * char容器转换为十六进制字符串
 * @tparam T 容器
 * @param data 数据
 * @return 十六进制字符串
 */
template <typename T>
std::string zs_char_container_to_hex_string(const T &data);

/**
 * 16进制字符串转为char容器
 * @tparam T 容器
 * @param str 字符串
 * @param data 用于返回的数据
 */
template <typename T>
void zs_hex_string_to_char_container(const std::string &str, T &data);

/**
 * @brief 创建实例的类
 * 从这个类继承，就可以获得make_instance接口
 * @tparam T 子类
 * @tparam zs_empty_class 父类
 */
template <typename T, typename Base = zs_empty_class>
struct zs_make_instance_t : public Base {
   private:
    zs_make_instance_t() = default;

   public:
    /**
     * @brief 创建对象
     *
     * @tparam Args 参数类型
     * @param args 参数
     * @return std::shared_ptr<T> 创建的对象
     */
    template <typename... Args>
    static std::shared_ptr<T> make_instance(Args &&... args) {
        return std::shared_ptr<T>(new T(std::forward<Args>(args)...));
    }

    using Base::Base;

    friend T;
};

std::string zs_safe_path(std::string danger_path);

}    // namespace zerone