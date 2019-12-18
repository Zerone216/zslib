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
 > FilePath: /github/zerone216/zslib/core/zs_object.hpp
 > CreateTime: 2019-12-18 20:03:19
 > ModifyTime: 2019-12-18 22:18:28
 > LastEditors: 
 > Description: 
 > version: 
 > Repository: https://github.com/zerone216/
 > Support: 
 > Others: 
 > Changelogs: 
 */

/*
 * zs_object 基础对象
 * 提供了信号---槽的通信机制、AOP编程
 */

#pragma once

#include "core/zs_eventloop.hpp"
#include "zs_object.h"
#include "tools/zs_random.hpp"

namespace zerone {

template <typename _VectorType, typename _FuncType>
int zs_object::__zs_bind_helper(std::recursive_mutex &mu, _VectorType &vec,
                                _FuncType func, bool single_thread) {
    std::lock_guard<std::recursive_mutex> lck(mu);
    int bind_id = zs_random::instance()->rand_int(0, INT_MAX);
    while (std::find_if(vec.begin(), vec.end(), [=](auto p) {
               return std::get<2>(p) == bind_id;
           }) != vec.end()) {
        bind_id = zs_random::instance()->rand_int(0, INT_MAX);
    }
    vec.push_back(std::make_tuple(std::function(func), single_thread, bind_id));
    return bind_id;
}

inline zs_object::~zs_object() { __p_msg_queue__->remove_msg(this); }

template <typename _VectorType>
void zs_object::__zs_aop_unbind_helper(std::recursive_mutex &mu,
                                       _VectorType &vec, int bind_id) {
    std::lock_guard<std::recursive_mutex> lck(mu);
    vec.erase(std::remove_if(vec.begin(), vec.end(),
                             [=](auto p) { return std::get<1>(p) == bind_id; }),
              vec.end());
}

template <typename _VectorType, typename _FuncType>
int zs_object::__zs_aop_after_add_helper(std::recursive_mutex &mu,
                                         _VectorType &vec, _FuncType func) {
    std::lock_guard<std::recursive_mutex> lck(mu);
    int bind_id = zs_random::instance()->rand_int(0, INT_MAX);
    while (std::find_if(vec.begin(), vec.end(), [=](auto p) {
               return std::get<1>(p) == bind_id;
           }) != vec.end()) {
        bind_id = zs_random::instance()->rand_int(0, INT_MAX);
    }
    vec.push_back(std::make_tuple(func, bind_id));
    return bind_id;
}

template <typename _VectorType, typename _FuncType>
int zs_object::__zs_aop_before_add_helper(std::recursive_mutex &mu,
                                          _VectorType &vec, _FuncType func) {
    std::lock_guard<std::recursive_mutex> lck(mu);
    int bind_id = zs_random::instance()->rand_int(0, INT_MAX);
    while (std::find_if(vec.begin(), vec.end(), [=](auto p) {
               return std::get<1>(p) == bind_id;
           }) != vec.end()) {
        bind_id = zs_random::instance()->rand_int(0, INT_MAX);
    }
    vec.insert(vec.begin(), std::make_tuple(func, bind_id));
    return bind_id;
}

template <typename _VectorType>
void zs_object::__zs_signal_unbind_helper(std::recursive_mutex &mu,
                                          _VectorType &vec, int bind_id) {
    std::lock_guard<std::recursive_mutex> lck(mu);
    vec.erase(std::remove_if(vec.begin(), vec.end(),
                             [=](auto p) { return std::get<2>(p) == bind_id; }),
              vec.end());
}

}    // namespace zerone
