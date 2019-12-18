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
 > FilePath: /github/zerone216/zslib/core/zs_timer.h
 > CreateTime: 2019-12-18 19:49:59
 > ModifyTime: 2019-12-18 21:50:03
 > LastEditors: 
 > Description: 
 > version: 
 > Repository: https://github.com/zerone216/
 > Support: 
 > Others: 
 > Changelogs: 
 */

/*
 * zs_timer 定时器
 */
#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include <atomic>
#include "core/zs_object.hpp"
#include "tools/zs_nocopy.h"

namespace zerone {
/**
 *  @brief 定时器
 */
class zs_timer : public zs_nocopy<zs_object> {
   public:
    /**
     * timeout 定时器触发信号
     */
    ZS_REG_SIGNAL(timeout);

   public:
    /**
     * @brief start 启动定时器
     * @param ms 毫秒
     * @param once 是否是一次性定时器
     */
    void start(int ms, bool once = false);

    /**
     * @brief stop 停止定时器
     */
    void stop();

    /**
     * @brief is_active 是否是活动的定时器
     * @return 是否活动
     */
    bool is_active() const;

   private:
    std::atomic<bool> running__{false};
    std::thread::id current_timer_thread__;
};

}    // namespace zerone

#pragma clang diagnostic pop