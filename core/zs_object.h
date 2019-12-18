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
 > FilePath: /github/zerone216/zslib/core/zs_object.h
 > CreateTime: 2019-12-18 19:36:32
 > ModifyTime: 2019-12-18 22:21:49
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
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#include "core/zs_msg_queue.hpp"
#include "zs_stdc++.h"
#include "tools/zs_json.hpp"

/*
 * ZS_REG_SIGNAL 注册信号的宏
 */
#define ZS_REG_SIGNAL(name, ...)                                           \
   public:                                                                 \
    std::recursive_mutex __mu_##name##_signal_;                            \
    std::vector<std::tuple<std::function<void(__VA_ARGS__)>, bool, int>>   \
        __##name##_signal_func_vec__;                                      \
    template <typename... __ZS_OBJECT_ARGS__>                              \
    void name(__ZS_OBJECT_ARGS__ &&... args) {                             \
        std::lock_guard<std::recursive_mutex> lck(__mu_##name##_signal_);  \
        for (auto &p : __##name##_signal_func_vec__) {                     \
            if (std::get<1>(p)) {                                          \
                std::get<0>(p)(std::forward<__ZS_OBJECT_ARGS__>(args)...); \
            } else {                                                       \
                auto bind_obj =                                            \
                    std::bind(std::get<0>(p),                              \
                              std::forward<__ZS_OBJECT_ARGS__>(args)...);  \
                __p_msg_queue__->add_msg(this, bind_obj);                  \
            }                                                              \
        }                                                                  \
    }

/*
 * ZS_REG_AOP 注册AOP的宏
 */
#define ZS_REG_AOP(name, ...)                                                 \
   public:                                                                    \
    std::recursive_mutex __mu_##name##_aop__;                                 \
    std::vector<std::tuple<std::function<void(__VA_ARGS__)>, int>>            \
        __##name##_aop_before_func_vec__;                                     \
    std::vector<std::tuple<std::function<void()>, int>>                       \
        __##name##_aop_after_func_vec__;                                      \
    template <typename... __ZS_OBJECT_ARGS__>                                 \
    decltype(auto) aop_##name(__ZS_OBJECT_ARGS__ &&... args) {                \
        std::lock_guard<std::recursive_mutex> lck(__mu_##name##_aop__);       \
        for (auto &p : __##name##_aop_before_func_vec__) {                    \
            std::get<0>(p)(std::forward<__ZS_OBJECT_ARGS__>(args)...);        \
        }                                                                     \
        if constexpr (std::is_same<decltype(                                  \
                                       name(std::forward<__ZS_OBJECT_ARGS__>( \
                                           args)...)),                        \
                                   void>::value) {                            \
            name(std::forward<__ZS_OBJECT_ARGS__>(args)...);                  \
            for (auto &p : __##name##_aop_after_func_vec__) {                 \
                std::get<0>(p)();                                             \
            }                                                                 \
        } else {                                                              \
            decltype(auto) ret =                                              \
                name(std::forward<__ZS_OBJECT_ARGS__>(args)...);              \
            for (auto &p : __##name##_aop_after_func_vec__) {                 \
                std::get<0>(p)();                                             \
            }                                                                 \
            return ret;                                                       \
        }                                                                     \
    }

/*
 * zs_aop_before_bind AOP调用前注册
 */
#define zs_aop_before_bind(objptr, name, func) \
    (objptr)->__zs_aop_before_add_helper(      \
        (objptr)->__mu_##name##_aop__,         \
        (objptr)->__##name##_aop_before_func_vec__, func)

/*
 * zs_aop_after_bind AOP调用后注册
 */
#define zs_aop_after_bind(objptr, name, func) \
    (objptr)->__zs_aop_after_add_helper(      \
        (objptr)->__mu_##name##_aop__,        \
        (objptr)->__##name##_aop_after_func_vec__, func)

/*
 * zs_aop_before_unbind AOP调用前反注册
 */
#define zs_aop_before_unbind(objptr, name, bind_id) \
    (objptr)->__zs_aop_unbind_helper(               \
        (objptr)->__mu_##name##_aop__,              \
        (objptr)->__##name##_aop_before_func_vec__, bind_id);

/*
 * zs_aop_after_unbind AOP调用后反注册
 */
#define zs_aop_after_unbind(objptr, name, bind_id) \
    (objptr)->__zs_aop_unbind_helper(              \
        (objptr)->__mu_##name##_aop__,             \
        (objptr)->__##name##_aop_after_func_vec__, bind_id);

/*
 * zs_bind_signal 信号绑定
 */
#define zs_bind_signal(objptr, name, func, single_thread)                    \
    (objptr)->__zs_bind_helper((objptr)->__mu_##name##_signal_,              \
                               (objptr)->__##name##_signal_func_vec__, func, \
                               single_thread)

/*
 * zs_unbind_signal 信号解绑
 */
#define zs_unbind_signal(objptr, name, bind_id) \
    (objptr)->__zs_signal_unbind_helper(        \
        (objptr)->__mu_##name##_signal_,        \
        (objptr)->__##name##_signal_func_vec__, bind_id);

namespace zerone {
/**
 *  @brief 元对象
 */
class zs_object {
   public:
    template <typename _VectorType, typename _FuncType>
    int __zs_bind_helper(std::recursive_mutex &mu, _VectorType &vec,
                         _FuncType func, bool single_thread);

    template <typename _VectorType>
    void __zs_signal_unbind_helper(std::recursive_mutex &mu, _VectorType &vec,
                                   int bind_id);

    template <typename _VectorType, typename _FuncType>
    int __zs_aop_before_add_helper(std::recursive_mutex &mu, _VectorType &vec,
                                   _FuncType func);

    template <typename _VectorType, typename _FuncType>
    int __zs_aop_after_add_helper(std::recursive_mutex &mu, _VectorType &vec,
                                  _FuncType func);

    template <typename _VectorType>
    void __zs_aop_unbind_helper(std::recursive_mutex &mu, _VectorType &vec,
                                int bind_id);

    virtual ~zs_object();

   protected:
    zs_msg_queue *__p_msg_queue__ = zs_msg_queue::instance();
};

}    // namespace zerone

#pragma clang diagnostic pop