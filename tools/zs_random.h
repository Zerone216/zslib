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
 > FilePath: /zerone216/zslib/tools/zs_random.h
 > CreateTime: 2020-01-07 19:34:51
 > ModifyTime: 2020-01-08 22:44:47
 > LastEditors: 
 > Description: 
 > version: 1.0.0
 > Repository: https://github.com/zerone216/
 > Support: 
 > Others: 
 > Changelogs: 
 */


#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include "core/zs_stdc++.h"
#include "zs_single_instance.h"
#include "tools/zs_nocopy.h"

namespace zerone {
/**
 *  @brief 随机数类
 */
class zs_random final : public zs_nocopy<> {
   public:
    SF_SINGLE_TON(zs_random)

   private:
    std::random_device rd__;
    std::shared_ptr<std::default_random_engine> e__;

    zs_random();

   public:
    /**
     * 获取随机整数
     * @param min 最小值
     * @param max 最大值
     * @return 随机数
     */
    int rand_int(int min, int max) const;
    /**
     * 获取随机浮点数
     * @param min 最小值
     * @param max 最大值
     * @return 随机数
     */
    double rand_double(double min, double max) const;
    /**
     * 获取uuid字符串
     * @return uuid字符串
     */
    std::string uuid_str() const;
};

}    // namespace zerone
#pragma clang diagnostic pop