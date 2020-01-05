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
 > FilePath: /zerone216/zslib/tools/zs_meta.hpp
 > CreateTime: 2020-01-03 19:06:46
 > ModifyTime: 2020-01-05 21:49:54
 > LastEditors: 
 > Description: 
 > version: 1.0.0
 > Repository: https://github.com/zerone216/
 > Support: 
 > Others: 
 > Changelogs: 
 */

/*
 * zs_meta 元编程辅助函数
 */
#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include "zs_meta.h"

namespace zerone {

template <int N>
auto make_placeholders() {
    return typename decltype(
        __zs_placeholders_type(std::placeholders::_1))::template type<N>{};
}

template <typename _Type, typename Tuple>
_Type *zs_make_obj_from_tuple(Tuple &&t) {
    constexpr auto size =
        std::tuple_size<typename std::decay<Tuple>::type>::value;
    return zs_make_obj_from_tuple_impl<_Type>(std::forward<Tuple>(t),
                                              std::make_index_sequence<size>{});
}

template <typename _Type, typename Tuple, size_t... Index>
_Type *zs_make_obj_from_tuple_impl(Tuple &&t, std::index_sequence<Index...>) {
    return new _Type(std::get<Index>(std::forward<Tuple>(t))...);
}

template <typename Function, typename Tuple>
decltype(auto) zs_invoke(Function &&func, Tuple &&t) {
    constexpr auto size =
        std::tuple_size<typename std::decay<Tuple>::type>::value;
    return zs_invoke_impl(std::forward<Function>(func), std::forward<Tuple>(t),
                          std::make_index_sequence<size>{});
}

template <typename Function, typename Tuple, size_t... Index>
decltype(auto) zs_invoke_impl(Function &&func, Tuple &&t,
                              std::index_sequence<Index...>) {
    if constexpr (std::is_same<decltype(func(
                                   std::get<Index>(std::forward<Tuple>(t))...)),
                               void>::value) {
        func(std::get<Index>(std::forward<Tuple>(t))...);
        return 0;
    } else {
        return func(std::get<Index>(std::forward<Tuple>(t))...);
    }
}

}    // namespace zerone

#pragma clang diagnostic pop