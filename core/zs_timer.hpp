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
 > FilePath: /github/zerone216/zslib/core/zs_timer.hpp
 > CreateTime: 2019-12-18 19:33:10
 > ModifyTime: 2019-12-18 21:53:19
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

#include "zs_timer.h"

namespace zerone {

inline void zs_timer::start(int ms, bool once) {
    if (running__) {
        return;
    }
    running__ = true;

    std::thread new_thread = std::thread(
        [this, ms](bool is_once) {
            while (true) {
                std::this_thread::sleep_for(std::chrono::milliseconds(ms));
                if (std::this_thread::get_id() != current_timer_thread__) {
                    return;
                }
                if (running__) {
                    timeout();
                    if (is_once) {
                        running__ = false;
                        return;
                    }
                } else {
                    break;
                }
            }
        },
        once);
    current_timer_thread__ = new_thread.get_id();
    new_thread.detach();
}

inline bool zs_timer::is_active() const { return running__; }

inline void zs_timer::stop() { running__ = false; }
}    // namespace zerone

#pragma clang diagnostic pop