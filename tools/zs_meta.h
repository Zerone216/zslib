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
 > FilePath: /zerone216/zslib/tools/zs_meta.h
 > CreateTime: 2020-01-02 18:54:48
 > ModifyTime: 2020-01-05 21:48:20
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
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "google-explicit-constructor"
#pragma ide diagnostic ignored "readability-redundant-declaration"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include "core/zs_stdc++.h"

namespace zerone {

template <class T>
struct zs_check_param_reference;

template <class T, class... U>
struct zs_check_param_reference<std::function<T(U...)>> final {
    static constexpr bool value =
        std::disjunction<std::is_reference<U>...>::value;
    // static constexpr bool value = (std::is_reference<U>::value || ... ||
    // false);
};

template <typename T>
struct zs_check_param_reference final {
    static constexpr bool value = zs_check_param_reference<decltype(
        std::function(std::declval<T>()))>::value;
    // static constexpr bool value = (std::is_reference<V>::value || ... ||
    // false);
};

template <class T>
struct zs_check_param_pointer;

template <class T, class... U>
struct zs_check_param_pointer<std::function<T(U...)>> final {
    static constexpr bool value =
        std::disjunction<std::is_pointer<U>...>::value;
    // static constexpr bool value = (std::is_pointer<U>::value || ... ||
    // false);
};

template <typename T>
struct zs_check_param_pointer final {
    static constexpr bool value = zs_check_param_pointer<decltype(
        std::function(std::declval<T>()))>::value;
    // static constexpr bool value = (std::is_pointer<V>::value || ... ||
    // false);
};

template <class T>
struct zs_check_return_reference final {};

template <class T, class... U>
struct zs_check_return_reference<std::function<T(U...)>> final {
    static constexpr bool value = std::is_reference<T>::value;
};

template <template <typename> class T, typename U, typename... V>
struct zs_check_return_reference<T<U(V...)>> final {
    static constexpr bool value = std::is_reference<U>::value;
};

template <class T>
struct zs_check_return_pointer final {};

template <class T, class... U>
struct zs_check_return_pointer<std::function<T(U...)>> final {
    static constexpr bool value = std::is_pointer<T>::value;
};

template <template <typename> class T, typename U, typename... V>
struct zs_check_return_pointer<T<U(V...)>> final {
    static constexpr bool value = std::is_pointer<U>::value;
};

template <typename T>
struct zs_function_type_helper;

template <class T, class... U>
struct zs_function_type_helper<std::function<T(U...)>> final {
    using return_type = typename std::decay<T>::type;
    using param_type = std::tuple<typename std::decay<U>::type...>;
};

template <typename T>
struct zs_function_type_helper final {
    using return_type = typename zs_function_type_helper<decltype(
        std::function(std::declval<T>()))>::return_type;
    using param_type = typename zs_function_type_helper<decltype(
        std::function(std::declval<T>()))>::param_type;
};

template <typename Function, typename Tuple, std::size_t... Index>
decltype(auto) zs_invoke_impl(Function &&func, Tuple &&t,
                              std::index_sequence<Index...>);

template <typename Function, typename Tuple>
decltype(auto) zs_invoke(Function &&func, Tuple &&t);

template <typename _Type, typename Tuple, std::size_t... Index>
_Type *zs_make_obj_from_tuple_impl(Tuple &&t, std::index_sequence<Index...>);

template <typename _Type, typename Tuple>
_Type *zs_make_obj_from_tuple(Tuple &&t);

//////////////////////////////////////////////////////////////////////

template <typename Function, typename Tuple, std::size_t... Index>
decltype(auto) zs_invoke_impl(Function &&func, Tuple &&t,
                              std::index_sequence<Index...>);

template <typename Function, typename Tuple>
decltype(auto) zs_invoke(Function &&func, Tuple &&t);

template <typename _Type, typename Tuple, std::size_t... Index>
_Type *zs_make_obj_from_tuple_impl(Tuple &&t, std::index_sequence<Index...>);

template <typename _Type, typename Tuple>
_Type *zs_make_obj_from_tuple(Tuple &&t);

/////////////////////////////////////////////////////////
// make_placeholders

template <template <int> typename _Placeholders>
struct __zs_placeholders_type {
    template <int N>
    __zs_placeholders_type(_Placeholders<N>){};
    template <int N>
    using type = _Placeholders<N>;
};

template <template <int> typename _Placeholders, int N>
__zs_placeholders_type(_Placeholders<N>)->__zs_placeholders_type<_Placeholders>;

template <int N>
auto make_placeholders();

template <typename T>
class zs_is_shared_ptr : public std::false_type {};

template <typename T>
class zs_is_shared_ptr<std::shared_ptr<T>> : public std::true_type {};

}    // namespace zerone

#pragma clang diagnostic pop