//
//  cache_impl.cpp
//  StmlCpp
//
//  Created by sun on 2016/12/14.
//  Copyright © 2016年 Smobiler. All rights reserved.
//

#include "cache_impl.hpp"

namespace smobiler {

    std::string CacheImpl::get(const std::string &key, const std::string &defaultVaue)
    {
        return defaultVaue;
    }
    
    bool CacheImpl::find(const std::string &key)
    {
        return false;
    }
    
    bool CacheImpl::del(const std::string &key)
    {
        return false;
    }
    
    bool CacheImpl::put(const std::string &key, const std::string &value)
    {
        return false;
    }
    
    
    std::string CacheImpl::getRootPath()
    {
        return "";
    }
}
