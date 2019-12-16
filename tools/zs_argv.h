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
 > FilePath: \22g:\IDV\zs_argv.h
 > CreateTime: 2019-12-14 11:45:06
 > ModifyTime: 2019-12-16 17:18:19
 > LastEditors: 
 > Description: 
 > version: 
 > Repository: https://github.com/zerone216/
 > Support: 
 > Others: 
 > Changelogs: 
 */

#pragma once

#include "core/zs_stdc++.h"
#include "zs_json.hpp"

namespace zerone {

/**
 * @brief 参数解析时的动作
 *
 */
enum class zs_argv_action {
    store,          //保存值
    store_true,     //保存为true
    store_false,    // 保存为false
    count           // 保存数量
};

/**
 * @brief 参数选项
 *
 */
struct zs_argv_opt_t {
    std::string short_name{};    // 短名称，如:-i
    std::string long_name{};     // 长名称，如:--id
    std::string json_name{};     // 保存在json中的键名称，如：id
    zs_json_type type = zs_json_type::string;    // 参数类型（参加zs_json_type）
    bool required = true;                        // 是否必须
    zs_argv_action action{zs_argv_action::store};    // 动作
    zs_json default_value{};                         // 默认值
};

/**
 * @brief 参数解析类
 *
 */
class zs_argparser : public std::enable_shared_from_this<zs_argparser> {
   private:
    std::unordered_map<std::string, std::shared_ptr<zs_argparser>> sub_parsers_;
    std::vector<zs_argv_opt_t> position_arg_;
    std::vector<zs_argv_opt_t> none_position_arg__;
    std::pair<std::string, std::string> prefix_;
    std::string help_;

    zs_json parse_argv__(const std::vector<std::string> &argv) const;

    zs_argparser(const std::string &help = "",
                 const std::pair<std::string, std::string> &prefix = {"-",
                                                                      "--"});

   public:
    /**
     * 创建一个解析器对象
     * @param help 帮助信息（TODO 输出help信息）
     * @param prefix 前缀
     * @return 解析器对象
     */
    static std::shared_ptr<zs_argparser> make_parser(
        const std::string &help = "",
        const std::pair<std::string, std::string> &prefix = {"-", "--"});

    /**
     * @brief 添加一个参数
     *
     * @param opt 参数信息
     * @return true 添加成功
     * @return false 添加失败
     */
    bool add_argument(zs_argv_opt_t opt);

    /**
     * @brief 添加一个参数
     *
     * @param short_name 短名称
     * @param long_name 长名称
     * @param type 类型
     * @param required 是否必须
     * @param default_value 默认值
     * @param json_name json键名称
     * @param action 动作
     * @return true 添加成功
     * @return false 添加失败
     */
    bool add_argument(std::string short_name, std::string long_name,
                      zs_json_type type = zs_json_type::string,
                      bool required = true, zs_json default_value = zs_json(),
                      std::string json_name = "",
                      zs_argv_action action = zs_argv_action::store);

    /**
     * @brief 添加子解析器
     *
     * @param name 解析器名称（会被用于键名称）
     * @param sub_parser 子解析器
     */
    void add_sub_parser(const std::string &name,
                        std::shared_ptr<zs_argparser> sub_parser);

    /**
     * @brief 解析参数
     *
     * @param argc 参数数量
     * @param argv 参数列表
     * @param skip0 跳过参数0
     * @return zs_json 解析的参数对象（json格式）
     */
    zs_json parse_argv(int argc, const char **argv, bool skip0 = true) const;
};

}    // namespace zerone
