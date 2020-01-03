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
 > FilePath: /zerone216/zslib/tools/zs_json.hpp
 > CreateTime: 2019-12-23 19:42:36
 > ModifyTime: 2020-01-03 22:32:37
 > LastEditors: 
 > Description: 
 > version: 
 > Repository: https://github.com/zerone216/
 > Support: 
 > Others: 
 > Changelogs: 
 */

#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-c-copy-assignment-signature"
#pragma ide diagnostic ignored "cert-err34-c"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include "compiler/zs_lex.hpp"
#include "compiler/zs_yacc.hpp"
#include "core/zs_define.h"
#include "core/zs_msvc_safe.h"
#include "zs_json.h"
#include "zs_logger.hpp"
#include "zs_meta.hpp"
#include "zs_utils.hpp"

namespace zerone {
inline zs_json zs_json::from_string(const std::string& json_str)
{
    zs_lex lex;
    lex.set_rules(
        { { "string",
              R"("([^\\"]|(\\["\\/bnrt]|(u[0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F])))*")" },
            { "[", R"(\[)" },
            { "]", R"(\])" },
            { "{", R"(\{)" },
            { "}", R"(\})" },
            { ",", R"(,)" },
            { ":", R"(:)" },
            { "ws", R"([\r\n\t ]+)" },
            { "number", R"(-?(0|[1-9]\d*)(\.\d+)?(e|E(\+|-)?0|[1-9]\d*)?)" },
            { "true", R"(true)" },
            { "false", R"(false)" },
            { "null", R"(null)" } });

    std::vector<zs_lex_result_t> lex_result;
    if (!lex.parse(json_str, lex_result)) {
        return zs_json();
    }
    lex_result.erase(
        std::remove_if(lex_result.begin(), lex_result.end(),
            [](const zs_lex_result_t& r) { return r.id == "ws"; }),
        lex_result.end());
    zs_yacc yacc;
    yacc.set_rules(
        { { "value",
              { { { "object" },
                    [](const std::vector<std::shared_ptr<zs_yacc_result_t>>& data)
                        -> std::any { return data[0]->user_data; } },
                  { { "array" },
                      [](const std::vector<std::shared_ptr<zs_yacc_result_t>>& data)
                          -> std::any { return data[0]->user_data; } },
                  { { "string" },
                      [](const std::vector<std::shared_ptr<zs_yacc_result_t>>& data)
                          -> std::any {
                          return zs_json(zs_json::json_string_to_string(data[0]->text));
                      } },
                  { { "number" },
                      [](const std::vector<std::shared_ptr<zs_yacc_result_t>>& data)
                          -> std::any {
                          return zs_json(zs_string_to_long_double(data[0]->text));
                      } },
                  { { "true" },
                      [](const std::vector<std::shared_ptr<zs_yacc_result_t>>& data)
                          -> std::any { return zs_json(true); } },
                  { { "false" },
                      [](const std::vector<std::shared_ptr<zs_yacc_result_t>>& data)
                          -> std::any { return zs_json(false); } },
                  { { "null" },
                      [](const std::vector<std::shared_ptr<zs_yacc_result_t>>& data)
                          -> std::any { return zs_json(); } } } },
            { "object",
                { { { "{", "}" },
                      [](const std::vector<std::shared_ptr<zs_yacc_result_t>>& data)
                          -> std::any {
                          zs_json json;
                          json.convert_to_object();
                          return json;
                      } },
                    { { "{", "members", "}" },
                        [](const std::vector<std::shared_ptr<zs_yacc_result_t>>& data)
                            -> std::any { return data[1]->user_data; } } } },
            { "members",
                { { { "member" },
                      [](const std::vector<std::shared_ptr<zs_yacc_result_t>>& data)
                          -> std::any { return data[0]->user_data; } },
                    { { "members", ",", "member" },
                        [](const std::vector<std::shared_ptr<zs_yacc_result_t>>& data)
                            -> std::any {
                            zs_json json;
                            json.convert_to_object();
                            const auto js1 = std::any_cast<zs_json>(data[0]->user_data);
                            const auto js2 = std::any_cast<zs_json>(data[2]->user_data);
                            // ReSharper disable once CppExpressionWithoutSideEffects
                            json.join(js1);
                            // ReSharper disable once CppExpressionWithoutSideEffects
                            json.join(js2);
                            return json;
                        } } } },
            { "member",
                { { { "string", ":", "value" },
                    [](const std::vector<std::shared_ptr<zs_yacc_result_t>>& data)
                        -> std::any {
                        zs_json json;
                        json.convert_to_object();
                        const auto json1 = std::any_cast<zs_json>(data[2]->user_data);
                        json[zs_json::json_string_to_string(data[0]->text)] = json1;
                        return json;
                    } } } },
            { "array",
                { { { "[", "]" },
                      [](const std::vector<std::shared_ptr<zs_yacc_result_t>>& data)
                          -> std::any {
                          zs_json json;
                          json.convert_to_array();
                          return json;
                      } },
                    { { "[", "values", "]" },
                        [](const std::vector<std::shared_ptr<zs_yacc_result_t>>& data)
                            -> std::any { return data[1]->user_data; } } } },
            { "values",
                { { { "value" },
                      [](const std::vector<std::shared_ptr<zs_yacc_result_t>>& data)
                          -> std::any {
                          zs_json json;
                          json.convert_to_array();
                          json.append(std::any_cast<zs_json>(data[0]->user_data));
                          return json;
                      } },
                    { { "values", ",", "value" },
                        [](const std::vector<std::shared_ptr<zs_yacc_result_t>>& data)
                            -> std::any {
                            zs_json json;
                            json.convert_to_array();
                            const auto json1 = std::any_cast<zs_json>(data[0]->user_data);
                            const auto json2 = std::any_cast<zs_json>(data[2]->user_data);
                            // ReSharper disable once CppExpressionWithoutSideEffects
                            json.join(json1);
                            json.append(json2);
                            return json;
                        } } } } });

    yacc.add_terminate_ids({ "value" });
    std::vector<std::shared_ptr<zs_yacc_result_t>> yacc_result;
    if (!yacc.parse(lex_result, yacc_result)) {
        return zs_json();
    }
    return std::any_cast<zs_json>(yacc_result[0]->user_data);
}

inline std::unordered_set<std::string> zs_json::keys() const
{
    std::unordered_set<std::string> ret;
    if (value__->type != zs_json_type::object) {
        return ret;
    }
    for (auto& p : value__->object_value) {
        ret.insert(p.first);
    }
    return ret;
}

inline zs_json::zs_json()
    : value__(std::make_shared<zs_json_value>())
{
    value__->type = zs_json_type::null;
}

inline zs_json::zs_json(const std::string& str)
    : zs_json()
{
    value__->type = zs_json_type::string;
    value__->value = str;
}

inline zs_json::zs_json(const char* c_str)
    : zs_json(std::string(c_str))
{
}

inline zs_json::zs_json(const zs_json& json)
    : zs_json()
{
    value__ = json.value__;
}

inline zs_json::zs_json(bool boolean_value)
    : zs_json()
{
    value__->type = zs_json_type::boolean;
    value__->value = boolean_value ? "true" : "false";
}

inline zs_json::zs_json(const std::shared_ptr<zs_json_value>& value)
{
    value__ = value;
}

inline void zs_json::convert_to_object() const
{
    if (value__->type == zs_json_type::object) {
        return;
    }
    value__->type = zs_json_type::object;
    clear();
}

inline void zs_json::convert_to_array() const
{
    if (value__->type == zs_json_type::array) {
        return;
    }
    value__->type = zs_json_type::array;
    clear();
}

inline zs_json_type zs_json::type() const { return value__->type; }

inline zs_json zs_json::at(const std::string& key) const
{
    if (value__->type != zs_json_type::object) {
        return zs_json();
    }
    if (value__->object_value.count(key) == 0) {
        return zs_json();
    }
    return zs_json(value__->object_value[key]);
}

inline zs_json zs_json::operator[](const std::string& key) const
{
    if (value__->type != zs_json_type::object) {
        convert_to_object();
    }
    if (value__->object_value.count(key) == 0) {
        value__->object_value[key] = zs_json().value__;
    }
    return zs_json(value__->object_value[key]);
}

inline zs_json zs_json::at(int key) const
{
    if (value__->type != zs_json_type::array) {
        return zs_json();
    }
    if (value__->array_value.size() <= key) {
        return zs_json();
    }
    return zs_json(value__->array_value[key]);
}

inline zs_json::operator std::string() const
{
    switch (value__->type) {
    case zs_json_type::array:
    case zs_json_type::object:
    case zs_json_type::null:
        return "";
    case zs_json_type::boolean:
    case zs_json_type::number:
    case zs_json_type::string:
        return value__->value;
    default:
        return "";
    }
}

inline std::string zs_json::json_string_to_string(std::string json_str)
{
    json_str.erase(json_str.begin());
    json_str.pop_back();
    zs_string_replace(json_str, "\\\"", "\"");
    zs_string_replace(json_str, "\\/", "/");
    zs_string_replace(json_str, "\\b", "\b");
    zs_string_replace(json_str, "\\f", "\f");
    zs_string_replace(json_str, "\\n", "\n");
    zs_string_replace(json_str, "\\r", "\r");
    zs_string_replace(json_str, "\\t", "\t");

    // todo 未处理unicode

    zs_string_replace(json_str, "\\\\", "\\");

    return json_str;
}

inline std::string zs_json::string_to_json_string(std::string str)
{
    zs_string_replace(str, "\\", "\\\\");
    zs_string_replace(str, "\"", "\\\"");
    zs_string_replace(str, "/", "\\/");
    zs_string_replace(str, "\b", "\\b");
    zs_string_replace(str, "\f", "\\f");
    zs_string_replace(str, "\n", "\\n");
    zs_string_replace(str, "\r", "\\r");
    zs_string_replace(str, "\t", "\\t");
    // todo 未处理unicode
    str.insert(str.begin(), '\"');
    str += "\"";
    return str;
}

inline zs_json::operator bool() const
{
    switch (value__->type) {
    case zs_json_type::array:
        return !value__->array_value.empty();
    case zs_json_type::object:
        return !value__->object_value.empty();
    case zs_json_type::null:
        return false;
    case zs_json_type::boolean:
        return value__->value == "true";
    case zs_json_type::number:
        return zs_string_to_long_double(value__->value) != 0;
    case zs_json_type::string:
        return !value__->value.empty();
    default:
        return false;
    }
}

template <typename T, typename>
zs_json& zs_json::operator=(T value)
{
    if (value__->type != zs_json_type::number) {
        value__->type = zs_json_type::number;
        clear();
    }
    value__->value = std::to_string(value);
    return *this;
}

inline zs_json& zs_json::operator=(const std::string& value)
{
    if (value__->type != zs_json_type::string) {
        value__->type = zs_json_type::string;
        clear();
    }
    value__->value = value;
    return *this;
}

inline void zs_json::append(const zs_json& value) const
{
    convert_to_array();
    value__->array_value.push_back(value.value__);
}

inline zs_json zs_json::deep_copy() const
{
    zs_json tmp_json;
    value_copy__(value__, tmp_json.value__);
    return tmp_json;
}

inline bool zs_json::join(const zs_json& other) const
{
    if (value__->type == zs_json_type::array && other.value__->type == zs_json_type::array) {
        value__->array_value.insert(value__->array_value.end(),
            other.value__->array_value.begin(),
            other.value__->array_value.end());
        return true;
    } else if (value__->type == zs_json_type::object && other.value__->type == zs_json_type::object) {
        for (auto& p : other.value__->object_value) {
            value__->object_value[p.first] = p.second;
        }
        return true;
    }
    return false;
}

inline void zs_json::clear() const
{
    value__->value.clear();
    value__->array_value.clear();
    value__->object_value.clear();
}

inline void zs_json::resize(const int size) const
{
    convert_to_array();
    value__->array_value.resize((unsigned long)size);
}

inline void zs_json::remove(const int pos) const
{
    if (value__->array_value.size() > pos) {
        value__->array_value.erase(value__->array_value.begin() + pos);
    }
}

inline void zs_json::remove(const int pos, int len) const
{
    if (value__->array_value.size() > pos) {
        if (len > value__->array_value.size() - pos) {
            len = static_cast<int>(value__->array_value.size() - pos);
        }
        value__->array_value.erase(value__->array_value.begin() + pos,
            value__->array_value.begin() + pos + len);
    }
}

inline void zs_json::remove(const std::string& key) const
{
    value__->object_value.erase(key);
}

inline std::string zs_json::to_string() const
{
    std::string ret;
    switch (value__->type) {
    case zs_json_type::object: {
        ret += "{";
        for (auto& p : value__->object_value) {
            ret += string_to_json_string(p.first) + ":" + zs_json(p.second).to_string() + ",";
        }
        if (ret.back() == ',')
            ret.pop_back();
        ret += "}";
    } break;
    case zs_json_type::array: {
        ret += "[";
        for (auto& p : value__->array_value) {
            ret += zs_json(p).to_string() + ",";
        }
        if (ret.back() == ',')
            ret.pop_back();
        ret += "]";
    } break;
    case zs_json_type::string:
        ret += string_to_json_string(value__->value);
        break;
    case zs_json_type::number: {
        ret += value__->value;
        break;
    }
    case zs_json_type::boolean:
        ret += value__->value.empty() ? "false" : value__->value;
        break;
    case zs_json_type::null:
        ret += "null";
        break;
    }
    return ret;
}

inline std::string zs_json::to_string(int indent, int current_indent) const
{
    std::string indent_string(current_indent, ' ');
    std::string ret;
    switch (value__->type) {
    case zs_json_type::object: {
        ret += "{\n";
        for (auto& p : value__->object_value) {
            ret += indent_string + std::string(indent, ' ') + string_to_json_string(p.first) + ":" + zs_json(p.second).to_string(indent, current_indent + indent) + ",\n";
        }
        if (!value__->object_value.empty()) {
            ret.erase(ret.end() - 2);
        }
        ret += indent_string + "}";
    } break;
    case zs_json_type::array: {
        ret += "[\n";
        for (auto& p : value__->array_value) {
            ret += indent_string + std::string(indent, ' ') + zs_json(p).to_string(indent, current_indent + indent) + ",\n";
        }
        if (!value__->object_value.empty()) {
            ret.erase(ret.end() - 2);
        }
        ret += indent_string + "]";
    } break;
    case zs_json_type::string:
        ret += string_to_json_string(value__->value);
        break;
    case zs_json_type::number: {
        ret += value__->value;
        break;
    }
    case zs_json_type::boolean:
        ret += (value__->value.empty() ? "false" : value__->value);
        break;
    case zs_json_type::null:
        ret += "null";
        break;
    }
    return ret;
}

inline zs_json& zs_json::operator=(const zs_json& value)
{
    if (&value != this) {
        value__->type = value.value__->type;
        value__->value = value.value__->value;
        value__->array_value = value.value__->array_value;
        value__->object_value = value.value__->object_value;
    }
    return *this;
}

inline void zs_json::copy(const zs_json& src)
{
    if (this != &src) {
        value__ = src.value__;
    }
}

inline void zs_json::value_copy__(const std::shared_ptr<zs_json_value>& src,
    std::shared_ptr<zs_json_value>& dst) const
{
    dst->type = src->type;
    dst->value = src->value;
    if (!src->array_value.empty()) {
        for (auto i = 0; i < src->array_value.size(); ++i) {
            dst->array_value.push_back(std::make_shared<zs_json_value>());
            value_copy__(src->array_value[i], dst->array_value[i]);
        }
    }
    if (!src->object_value.empty()) {
        for (auto& p : src->object_value) {
            dst->object_value[p.first] = std::make_shared<zs_json_value>();
            value_copy__(p.second, dst->object_value[p.first]);
        }
    }
}

inline zs_json zs_json::at(const char* c_key) const
{
    return at(std::string(c_key));
}

inline zs_json zs_json::operator[](const char* c_key) const
{
    return operator[](std::string(c_key));
}

inline zs_json zs_json::operator[](const int key) const
{
    if (value__->type != zs_json_type::array) {
        convert_to_array();
    }
    if (value__->array_value.size() <= key) {
        resize(key + 1);
    }
    return zs_json(value__->array_value[key]);
}

inline zs_json& zs_json::operator=(const bool value)
{
    if (value__->type != zs_json_type::boolean) {
        value__->type = zs_json_type::boolean;
        clear();
    }
    value__->value = value ? "true" : "false";
    return *this;
}

inline bool zs_json::is_null() const
{
    return value__->type == zs_json_type::null;
}

inline zs_json& zs_json::operator=(const char* value)
{
    return operator=(std::string(value));
}

inline void zs_json::convert_to_null() const
{
    clear();
    value__->type = zs_json_type ::null;
}

inline size_t zs_json::size() const
{
    return value__->type == zs_json_type ::array ? value__->array_value.size()
                                                 : 0;
}

inline bool zs_json::has(const std::string& key) const
{
    return value__->type == zs_json_type ::object
        ? value__->object_value.count(key) != 0
        : false;
}

inline bool zs_json::has(const char* c_key) const
{
    return has(std::string(c_key));
}

inline zs_json operator""_json(const char* str, std::size_t)
{
    return zs_json::from_string(str);
}

inline std::ostream& operator<<(std::ostream& os, const zs_json& json)
{
    return os << json.to_string();
}

template <typename T>
zs_json to_json(const T& t)
{
    return zs_json(t);
}

template <typename T>
zs_json to_json(std::shared_ptr<T> pt)
{
    return zs_json(*pt);
}

template <typename T, typename>
zs_json::operator T() const
{
    switch (value__->type) {
    case zs_json_type::array:
    case zs_json_type::object:
    case zs_json_type::null:
    case zs_json_type::string:
        return 0;
    case zs_json_type::boolean:
        return value__->value == "true";
    case zs_json_type::number:
        return static_cast<T>(zs_string_to_long_double(value__->value));
    default:
        return 0;
    }
}

template <typename T, typename>
zs_json::zs_json(T number)
    : zs_json()
{
    value__->type = zs_json_type::number;
    value__->value = zs_long_double_to_string(number);
}

template <typename T>
zs_json zs_to_json_helper__(const std::string& key, const T& value)
{
    zs_json js;
    js.convert_to_object();
    js[key] = to_json(value);
    return js;
}

template <typename T, typename... ARGS>
zs_json zs_to_json_helper__(const std::string& key, const T& value,
    const ARGS&... args)
{
    zs_json js;
    js.convert_to_object();
    js[key] = to_json(value);
    js.join(zs_to_json_helper__(args...));
    return js;
}

template <typename T>
void zs_from_json_helper__(const zs_json& js, const std::string& key,
    T& value)
{
    from_json(js.at(key), value);
}

template <typename T, typename... ARGS>
void zs_from_json_helper__(const zs_json& js, const std::string& key, T& value,
    ARGS&&... args)
{
    from_json(js.at(key), value);
    zs_from_json_helper__(js, std::forward<ARGS>(args)...);
}

template <typename... ARGS>
void from_json(const zs_json& js, std::tuple<ARGS...>& value)
{
    from_json_tuple_helper__<0>(js, value);
}

template <typename T>
void from_json(const zs_json& js, T& value)
{
    value = static_cast<T>(js);
}

template <typename T>
void from_json(const zs_json& js, std::shared_ptr<T>& value)
{
    value = std::make_shared<T>(static_cast<T>(js));
}

template <typename K, typename V>
zs_json to_json(const std::pair<K, V>& value)
{
    zs_json js;
    js.convert_to_object();
    js["key"] = to_json(value.first);
    js["value"] = to_json(value.second);
    return js;
}

template <typename K, typename V>
void from_json(const zs_json& js, std::pair<K, V>& value)
{
    K k;
    V v;
    from_json(js.at("key", k));
    from_json(js.at("value", v));
    value = { k, v };
}

template <typename... ARGS>
zs_json to_json(const std::tuple<ARGS...>& value)
{
    return zs_invoke(to_json_tuple_helper__<ARGS...>, value);
}

template <typename... ARGS>
zs_json to_json_tuple_helper__(const ARGS&... value)
{
    zs_json js;
    js.convert_to_array();
    (js.append(to_json(value)), ...);
    return js;
}

template <int N, typename... ARGS, typename... Ret>
std::enable_if_t<N != sizeof...(ARGS), void> from_json_tuple_helper__(
    const zs_json& js, std::tuple<ARGS...>& data, Ret... ret)
{
    std::remove_reference_t<decltype(std::get<N>(data))> t_data;
    from_json(js.at(N), t_data);
    from_json_tuple_helper__<N + 1>(js, data, ret..., t_data);
}

template <int N, typename... ARGS, typename... Ret>
std::enable_if_t<N == sizeof...(ARGS), void> from_json_tuple_helper__(
    const zs_json& js, std::tuple<ARGS...>& data, Ret... ret)
{
    data = { ret... };
}

#define SF_CONTAINER_JSON_IMPL(container)                            \
    template <typename T>                                            \
    inline zs_json to_json(const container<T>& value)                \
    {                                                                \
        zs_json js;                                                  \
        js.convert_to_array();                                       \
        for (auto& p : value) {                                      \
            js.append(to_json(p));                                   \
        }                                                            \
        return js;                                                   \
    }                                                                \
    template <>                                                      \
    inline zs_json to_json(const container<char>& value)             \
    {                                                                \
        return zs_json(zs_char_container_to_hex_string(value));      \
    }                                                                \
    template <typename T>                                            \
    inline void from_json(const zs_json& js, container<T>& value)    \
    {                                                                \
        std::vector<T> data;                                         \
        int sz = js.size();                                          \
        data.resize(sz);                                             \
        for (int i = 0; i < sz; ++i) {                               \
            from_json<T>(js.at(i), data[i]);                         \
        }                                                            \
        value = container<T> { data.begin(), data.end() };           \
    }                                                                \
    template <>                                                      \
    inline void from_json(const zs_json& js, container<char>& value) \
    {                                                                \
        std::string tmp;                                             \
        from_json(js, tmp);                                          \
        zs_hex_string_to_char_container(tmp, value);                 \
    }

#define SF_ASSOCIATED_CONTAINER_JSON_IMPL(container)             \
    template <typename K, typename V>                            \
    zs_json to_json(const container<K, V>& value)                \
    {                                                            \
        zs_json js;                                              \
        js.convert_to_array();                                   \
        for (auto& p : value) {                                  \
            zs_json tmp_js;                                      \
            tmp_js.convert_to_object();                          \
            tmp_js["key"] = to_json(p.first);                    \
            tmp_js["value"] = to_json(p.second);                 \
            js.append(tmp_js);                                   \
        }                                                        \
        return js;                                               \
    }                                                            \
    template <typename K, typename V>                            \
    void from_json(const zs_json& js, container<K, V>& value)    \
    {                                                            \
        std::vector<std::pair<K, V>> data;                       \
        int sz = js.size();                                      \
        for (int i = 0; i < sz; ++i) {                           \
            data.push_back(                                      \
                std::make_pair(from_json<K>(js.at(i).at("key")), \
                    from_json<V>(js.at(i).at("value"))));        \
        }                                                        \
        value = container<K, V> { data.begin(), data.end() };    \
    }

SF_CONTAINER_JSON_IMPL(std::vector)
SF_CONTAINER_JSON_IMPL(std::list)
SF_CONTAINER_JSON_IMPL(std::deque)
SF_CONTAINER_JSON_IMPL(std::set)
SF_CONTAINER_JSON_IMPL(std::multiset)
SF_CONTAINER_JSON_IMPL(std::unordered_set)
SF_CONTAINER_JSON_IMPL(std::unordered_multiset)

SF_ASSOCIATED_CONTAINER_JSON_IMPL(std::map)
SF_ASSOCIATED_CONTAINER_JSON_IMPL(std::multimap)
SF_ASSOCIATED_CONTAINER_JSON_IMPL(std::unordered_map)
SF_ASSOCIATED_CONTAINER_JSON_IMPL(std::unordered_multimap)

#undef SF_CONTAINER_JSON_IMPL
#undef SF_ASSOCIATED_CONTAINER_JSON_IMPL

} // namespace zerone

#pragma clang diagnostic pop