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
 > FilePath: /github/zerone216/zslib/tools/zs_utils.hpp
 > CreateTime: 2019-12-18 21:43:57
 > ModifyTime: 2019-12-18 22:31:36
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
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include <sys/stat.h>
#include <sys/types.h>
#include "core/zs_stdc++.h"

#include "core/zs_msvc_safe.h"
#include "core/zs_type.hpp"
#include "zs_utils.h"

namespace zerone {
inline std::string zs_string_trim(const std::string &str) {
    if (str.length() == 0) return "";
    unsigned long begin = 0;
    for (unsigned long i = 0; i < str.length(); ++i) {
        if (!isspace(str[i])) {
            begin = i;
            break;
        }
    }
    unsigned long end = begin;
    for (unsigned long i = str.length(); i > begin; --i) {
        if (!isspace(str[i - 1])) {
            end = i;
            break;
        }
    }
    return {str.begin() + begin, str.begin() + end};
}

inline std::string zs_to_lower_string(std::string str) {
    for (auto &p : str) p = static_cast<char>(tolower(p));
    return str;
}

inline std::string zs_get_path_ext(const std::string &path) {
    auto pos = path.rfind('.');
    if (pos == -1) return "";
    return std::string(path.begin() + pos + 1, path.end());
}

inline bool zs_equal_nocase_string(const std::string &str1,
                                   const std::string &str2) {
    return zs_to_lower_string(str1) == zs_to_lower_string(str2);
}

inline std::vector<std::string> zs_split_string(std::string str,
                                                const std::string &split_str) {
    std::vector<std::string> ret;
    unsigned long pos = 0;
    while (std::string::npos != (pos = str.find(split_str))) {
        ret.emplace_back(str.begin(), str.begin() + pos);
        str = std::string(str.begin() + pos + split_str.length(), str.end());
    }
    ret.push_back(str);
    return ret;
}

inline bool zs_write_file(const std::string &file_name, const byte_array &data,
                          bool append) {
    std::ofstream fo(
        file_name, (append ? std::ios::app : std::ios::out) | std::ios::binary);
    if (!fo) return false;
    fo.write(data.data(), data.size());
    fo.close();
    return true;
}

inline bool zs_read_file(const std::string &file_name, byte_array &data) {
    std::ifstream fi(file_name, std::ios::in | std::ios::binary);
    if (!fi) return false;
    fi.seekg(0, std::ios::end);
    auto size = fi.tellg();
    data.resize(static_cast<unsigned long>(size));
    fi.seekg(0, std::ios::beg);
    fi.read(data.data(), size);
    fi.close();
    return true;
}

inline void zs_string_replace(std::string &str, const std::string &from,
                              const std::string &to) {
    if (str.empty()) return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

inline std::string zs_long_double_to_string(const long double &num) {
    auto buffer = std::to_string(num);
    if (buffer.length() > 7) {
        if (std::string(buffer.end() - 7, buffer.end()) == ".000000") {
            buffer = {buffer.begin(), buffer.end() - 7};
        }
    }
    return buffer;
}

inline long double zs_string_to_long_double(const std::string &str) {
    long double tmp_num;
#ifdef MSC_VER
    sscanf_s(str.c_str(), "%Lf", &tmp_num);
#else
    sscanf(str.c_str(), "%Lf", &tmp_num);
#endif    // MSC_VER
    return tmp_num;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedValue"

inline std::string zs_make_time_str(
    const std::chrono::system_clock::time_point &tp) {
    auto tt = std::chrono::system_clock::to_time_t(tp);
    tm tm_d;
    auto ptm = &tm_d;
#ifdef _MSC_VER
    localtime_s(ptm, &tt);
#else
    ptm = localtime(&tt);
#endif
    std::ostringstream os;
    os << std::put_time(ptm, "%Y-%m-%d %H:%M:%S");
    return os.str();
}

#pragma clang diagnostic pop

template <typename T>
std::string zs_char_container_to_hex_string(const T &data) {
    std::string str;
    str.reserve(data.size() * 2 + 1);
    char tmp[4];
    for (auto &p : data) {
        zs_safe_sprintf(tmp, 4, "%02x", p);
        str += tmp;
    }
    return str;
}

template <typename T>
void zs_hex_string_to_char_container(const std::string &str, T &data) {
    data.clear();
    if (str.size() % 2 != 0) {
        return;
    }
    std::vector<char> tmp_ret_data(str.size() / 2);
    for (auto i = 0; i < str.length() / 2; ++i) {
        int tmp;
#ifdef _MSC_VER
        sscanf_s(str.data() + i * 2, "%02x", &tmp);
#else
        sscanf(str.data() + i * 2, "%02x", &tmp);
#endif    // _MSC_VER
        tmp_ret_data[i] = tmp;
    }
    data = T(tmp_ret_data.begin(), tmp_ret_data.end());
}

inline bool zs_string_start_with(const std::string &str,
                                 const std::string &prefix) {
    return str.find(prefix) == 0;
}

inline bool zs_string_end_with(const std::string &str,
                               const std::string &suffix) {
    return str.find_last_of(suffix) == str.length() - suffix.size();
}

inline std::string zs_safe_path(std::string danger_path) {
    zs_string_replace(danger_path,
                      std::string(1, fs::path::preferred_separator), "");
    return danger_path;
}

}    // namespace zerone
#pragma clang diagnostic pop