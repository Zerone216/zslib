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
 > FilePath: /github/zerone216/zslib/tools/zs_logger.h
 > CreateTime: 2019-12-18 22:23:21
 > ModifyTime: 2019-12-18 22:57:16
 > LastEditors: 
 > Description: 
 > version: 
 > Repository: https://github.com/zerone216/
 > Support: 
 > Others: 
 > Changelogs: 
 */

/*
 * zs_logger日志打印
 */

#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include "core/zs_stdc++.h"

#ifdef QT_CORE_LIB
#include <QString>
#endif

#include "zs_random.hpp"
#include "zs_single_instance.hpp"
#include "zs_utils.hpp"

namespace zerone {

/**
 * @brief  日志等级
 */
enum ZS_LOG_LEVEL {
    ZS_DEBUG_LEVEL = 0,
    ZS_INFO_LEVEL = 1,
    ZS_WARN_LEVEL = 2,
    ZS_ERROR_LEVEL = 3,
    ZS_FATAL_LEVEL = 4
};

/**
 *  @brief 日志信息
 */
struct zs_logger_info_t__ {
    ZS_LOG_LEVEL level;           // 日志等级
    std::string time;             // 时间
    int line;                     // 行号
    std::string file;             // 文件名称
    std::thread::id thread_id;    // 线程号
    std::string func;             // 函数名称
    std::string msg;              // 消息
};

/**
 * 各种level对应的字符串
 */
inline std::unordered_map<ZS_LOG_LEVEL, std::string> logger_level_str__{
    {ZS_DEBUG_LEVEL, "DEBUG"}, 
    {ZS_INFO_LEVEL, "INFO "},
    {ZS_WARN_LEVEL, "WARN "},  
    {ZS_ERROR_LEVEL, "ERROR"},
    {ZS_FATAL_LEVEL, "FATAL"},
};

/**
 *  @brief 默认日志格式
 */
constexpr char zs_default_log_format[] =
    "[{level}][{time}][{thread}][{file} ({line}) {func}] --> {msg}\n";

/**
 * 日志类
 */
class zs_logger {
   public:
    ZS_SINGLE_TON(zs_logger)

    /**
     * 添加指定等级的回调函数
     * @param level 等级
     * @param func 函数
     * @return id号（可用于移除回调）
     */
    int add_level_func(ZS_LOG_LEVEL level,
                       std::function<void(const zs_logger_info_t__ &)> func);

    /**
     * 添加指定等级日志输出流
     * @param level 等级
     * @param os 输出流
     * @param format 格式化字符串
     * @return id号（可用于移除回调）
     */
    int add_level_stream(ZS_LOG_LEVEL level, std::ostream *os,
                         std::string format = zs_default_log_format);

    /**
     * 添加指定等级日志输出文件
     * @param level 等级
     * @param filename 文件名
     * @param format 格式化字符串
     * @return id号（可用于移除回调）
     */
    int add_level_file(ZS_LOG_LEVEL level, const std::string &filename,
                       std::string format = zs_default_log_format);

    /**
     * 根据id删除过滤器
     * @param key id
     */
    void remove_filter(int key);

    template <typename T>
    void logout(ZS_LOG_LEVEL level, const std::string &file, int line,
                const std::string &func, const T &dt);

    template <typename... T>
    void logout(ZS_LOG_LEVEL level, const std::string &file, int line,
                const std::string &func, const T &... dt);

    void stop_logger();

    static void empty_func__() {}

    static std::string format(std::string format_str,
                              const zs_logger_info_t__ &log_info);

   private:
    std::deque<zs_logger_info_t__> log_deque__;
    std::condition_variable cond__;
    std::mutex deque_mu__;
    std::unordered_map<
        int, std::unordered_map<
                 int, std::function<void(const zs_logger_info_t__ &)>>>
        logger_func_set__;
    std::atomic<bool> run__{true};
    std::recursive_mutex func_set_mutex__;

    bool check_key_can_use__(int key);

    int make_random_logger_id__();

    zs_logger();

    template <typename T, typename... U>
    void logout__(std::ostringstream &oss, zs_logger_info_t__ &log_info,
                  const T &tmp, const U &... tmp2);

#ifdef QT_CORE_LIB
    template <typename... U>
    void logout__(std::ostringstream &oss, zs_logger_info_t__ &log_info,
                  const QString &tmp, const U &... tmp2);
    // template<>
    void logout__(std::ostringstream &oss, zs_logger_info_t__ &log_info,
                  const QString &tmp);
#endif
    template <typename T>
    void logout__(std::ostringstream &oss, zs_logger_info_t__ &log_info,
                  const T &tmp);
};

#ifdef ZS_DEBUG
#define zs_debug(...)                                                      \
    zerone::g_logger->logout(zerone::ZS_DEBUG_LEVEL, __FILE__, __LINE__, \
                              __FUNCTION__, __VA_ARGS__)
#else
#define zs_debug(...) zerone::g_logger->empty_func__()
#endif

#define zs_info(...)                                                      \
    zerone::g_logger->logout(zerone::ZS_INFO_LEVEL, __FILE__, __LINE__, \
                              __FUNCTION__, __VA_ARGS__)
#define zs_warn(...)                                                      \
    zerone::g_logger->logout(zerone::ZS_WARN_LEVEL, __FILE__, __LINE__, \
                              __FUNCTION__, __VA_ARGS__)
#define zs_error(...)                                                      \
    zerone::g_logger->logout(zerone::ZS_ERROR_LEVEL, __FILE__, __LINE__, \
                              __FUNCTION__, __VA_ARGS__)
#define zs_fatal(...)                                                      \
    zerone::g_logger->logout(zerone::ZS_FATAL_LEVEL, __FILE__, __LINE__, \
                              __FUNCTION__, __VA_ARGS__)

}    // namespace zerone

#pragma clang diagnostic pop