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
 > FilePath: /zerone216/zslib/tools/zs_finally.h
 > CreateTime: 2019-12-15 18:22:14
 > ModifyTime: 2020-01-03 22:24:21
 > LastEditors: 
 > Description: 
 > version: 1.0.0
 > Repository: https://github.com/zerone216/
 > Support: 
 > Others: 
 > Changelogs: 
 */

/*
 * zs_finally 清理过程
 */

#pragma once
#include <functional>

namespace zerone {
/**
 *  @brief 清理过程对象
 */
class zs_finally final {
   public:
    /**
     * @brief zs_finally 构造一个清理过程对象
     * @param func 作用域结束后会调用的函数
     */
    explicit zs_finally(std::function<void()> func);
    zs_finally(const zs_finally&) = delete;
    zs_finally(zs_finally&&) = delete;
    zs_finally& operator=(zs_finally&&) = delete;
    zs_finally& operator=(const zs_finally&) = delete;
    ~zs_finally();

   private:
    std::function<void()> func__;
};

}    // namespace zerone
