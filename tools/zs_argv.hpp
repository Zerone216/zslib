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
 > FilePath: /github/zerone216/zslib/tools/zs_argv.hpp
 > CreateTime: 2019-12-14 12:17:59
 > ModifyTime: 2019-12-16 22:48:10
 > LastEditors: 
 > Description: 
 > version: 
 > Repository: https://github.com/zerone216/
 > Support: 
 > Others: 
 > Changelogs: 
 */

#pragma once

#include "zs_argv.h"
#include "zs_utils.hpp"

namespace zerone {
inline zs_argparser::zs_argparser(
    const std::string &help, const std::pair<std::string, std::string> &prefix)
    : help_(help), prefix_(prefix) {}

inline std::shared_ptr<zs_argparser> zs_argparser::make_parser(
    const std::string &help,
    const std::pair<std::string, std::string> &prefix) {
    return std::shared_ptr<zs_argparser>(new zs_argparser(help, prefix));
}

inline void zs_argparser::add_sub_parser(
    const std::string &name,
    std::shared_ptr<zerone::zs_argparser> sub_parser) {
    sub_parsers_[name] = sub_parser;
}

inline bool zs_argparser::add_argument(zerone::zs_argv_opt_t opt) {
    if (opt.json_name.empty()) {
        opt.json_name = opt.long_name;
    }
    if (opt.json_name.empty()) {
        opt.json_name = opt.short_name;
    }
    if (opt.json_name.empty()) {
        return false;
    }
    for (zs_argv_opt_t &p : none_position_arg__) {
        if (opt.json_name == p.json_name) {
            return false;
        }
    }
    for (zs_argv_opt_t &p : position_arg_) {
        if (opt.json_name == p.json_name) {
            return false;
        }
    }
    if (zs_string_start_with(opt.short_name, prefix_.first) &&
        zs_string_start_with(opt.long_name, prefix_.second)) {
        for (zs_argv_opt_t &p : none_position_arg__) {
            if (p.short_name == opt.short_name) {
                return false;
            } else if (p.long_name == opt.short_name) {
                return false;
            } else if (p.short_name == opt.long_name) {
                return false;
            } else if (p.long_name == opt.long_name) {
                return false;
            }
        }
        none_position_arg__.push_back(opt);
    } else {
        for (zs_argv_opt_t &p : position_arg_) {
            if (p.short_name == opt.short_name) {
                return false;
            }
        }
        position_arg_.push_back(opt);
    }
    return true;
}

inline zs_json zs_argparser::parse_argv__(
    const std::vector<std::string> &argv) const {
    zs_json ret;
    ret.convert_to_object();
    std::shared_ptr<zs_argv_opt_t> last_opt;
    int pos = 0;
    for (auto &p : none_position_arg__) {
        if (!p.default_value.is_null()) {
            ret[p.json_name] = p.default_value;
        }
        if (p.action == zs_argv_action::count) {
            ret[p.json_name] = 0;
        }
        if (p.action == zs_argv_action::store_false) {
            ret[p.json_name] = true;
        }
        if (p.action == zs_argv_action::store_true) {
            ret[p.json_name] = false;
        }
    }
    for (const std::string &p : argv) {
        if (!last_opt) {
            bool find_flag = false;
            for (const zs_argv_opt_t &nopt : none_position_arg__) {
                if (nopt.long_name == p || nopt.short_name == p) {
                    find_flag = true;
                    if (nopt.action == zs_argv_action::store_true) {
                        ret[nopt.json_name] = true;
                        break;
                    } else if (nopt.action == zs_argv_action::store_false) {
                        ret[nopt.json_name] = false;
                        break;
                    } else if (nopt.action == zs_argv_action::count) {
                        ret[nopt.json_name] =
                            static_cast<int>(ret[nopt.json_name]) + 1;
                        break;
                    }
                    last_opt =
                        std::shared_ptr<zs_argv_opt_t>(new zs_argv_opt_t(nopt));
                    break;
                }
            }
            if (find_flag) {
                continue;
            }
            if (pos >= position_arg_.size()) {
                zs_debug("parse error");
                return zs_json();
            }
            auto d = zs_json();
            last_opt = std::shared_ptr<zs_argv_opt_t>(
                new zs_argv_opt_t(position_arg_[pos]));
            ++pos;
            switch (last_opt->type) {
                case zs_json_type::string:
                    d = p;
                    ret[last_opt->json_name] = d;
                    last_opt = nullptr;
                    break;
                case zs_json_type::array:
                    d = p;
                    ret[last_opt->json_name].append(zs_json(p));
                    break;
                case zs_json_type::boolean:
                    d = !(p == "0" || p == "false");
                    ret[last_opt->json_name] = d;
                    last_opt = nullptr;
                    break;
                case zs_json_type::number:
                    d = zs_string_to_long_double(p);
                    ret[last_opt->json_name] = d;
                    last_opt = nullptr;
                    break;
                default:
                    zs_debug("unsupport");
                    return zs_json();
            }
        } else {
            auto d = zs_json();

            if (last_opt->type == zs_json_type::array) {
                auto find_flag = false;
                for (const zs_argv_opt_t &nopt : none_position_arg__) {
                    if (nopt.long_name == p || nopt.short_name == p) {
                        find_flag = true;
                        if (nopt.action == zs_argv_action::store_true) {
                            ret[nopt.json_name] = true;
                            break;
                        } else if (nopt.action == zs_argv_action::store_false) {
                            ret[nopt.json_name] = false;
                            break;
                        } else if (nopt.action == zs_argv_action::count) {
                            ret[nopt.json_name] =
                                static_cast<int>(ret[nopt.json_name]) + 1;
                            break;
                        }
                        last_opt = std::shared_ptr<zs_argv_opt_t>(
                            new zs_argv_opt_t(nopt));
                        break;
                    }
                }
                if (find_flag) {
                    continue;
                }
            }

            switch (last_opt->type) {
                case zs_json_type::string:
                    d = p;
                    ret[last_opt->json_name] = d;
                    last_opt = nullptr;
                    break;
                case zs_json_type::array:
                    d = p;
                    ret[last_opt->json_name].append(d);
                    break;
                case zs_json_type::boolean:
                    d = !(p == "0" || p == "false");
                    ret[last_opt->json_name] = d;
                    last_opt = nullptr;
                    break;
                case zs_json_type::number:
                    d = zs_string_to_long_double(p);
                    ret[last_opt->json_name] = d;
                    last_opt = nullptr;
                    break;
                default:
                    zs_debug("unsupport");
                    return zs_json();
            }
        }
    }
    for (auto &p : none_position_arg__) {
        if (p.required) {
            if (!ret.has(p.json_name)) {
                zs_debug(p.short_name + "/" + p.long_name + " is required");
                return zs_json();
            }
        }
    }
    for (auto &p : position_arg_) {
        if (p.required) {
            if (!ret.has(p.json_name)) {
                zs_debug(p.short_name + "/" + p.long_name + " is required");
                return zs_json();
            }
        }
    }
    return ret;
}

inline zs_json zs_argparser::parse_argv(int argc, const char **argv,
                                        bool skip0) const {
    std::vector<std::string> data({argv + skip0, argv + argc});
    zs_json ret;
    zs_json curr = ret;
    curr.convert_to_object();
    auto parser = shared_from_this();
    for (int i = 0; i < data.size(); ++i) {
        bool find_flag = false;
        for (auto &p : parser->sub_parsers_) {
            if (p.first == data[i]) {
                find_flag = true;
                curr[p.first] = zs_json();
                curr.copy(curr[p.first]);
                curr.convert_to_object();
                parser = p.second;
                break;
            }
        }
        if (!find_flag) {
            curr.join(parser->parse_argv__({data.begin() + i, data.end()}));
            return ret;
        }
    }
    return zs_json();
}

bool zs_argparser::add_argument(std::string short_name, std::string long_name,
                                zs_json_type type, bool required,
                                zs_json default_value, std::string json_name,
                                zs_argv_action action) {
    zs_argv_opt_t opt;
    opt.short_name = short_name;
    opt.long_name = long_name;
    opt.json_name = json_name;
    opt.type = type;
    opt.action = action;
    opt.default_value = default_value;
    opt.required = required;
    return add_argument(opt);
}

}    // namespace zerone