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
 > FilePath: /zerone216/zslib/tools/zs_nocopy.h
 > CreateTime: 2020-01-03 19:11:37
 > ModifyTime: 2020-01-05 21:51:22
 > LastEditors: 
 > Description: 
 > version: 1.0.0
 > Repository: https://github.com/zerone216/
 > Support: 
 > Others: 
 > Changelogs: 
 */

/*
 * zs_nocopy 禁止拷贝
 */

#pragma once

#include "core/zs_empty_class.h"

namespace zerone {
/**
 * @brief 禁止拷贝工具类
 * @tparam _BaseClass 基类型
 */
template <typename _BaseClass = zs_empty_class>
class zs_nocopy : public _BaseClass {
   public:
    zs_nocopy(){};
    zs_nocopy(const zs_nocopy&) = delete;
    zs_nocopy(zs_nocopy&&) = delete;
    zs_nocopy& operator=(const zs_nocopy&) = delete;
    zs_nocopy& operator=(zs_nocopy&&) = delete;
};
}    // namespace zerone
