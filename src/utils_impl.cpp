//
//  utils_impl.cpp
//  StmlCpp
//
//  Created by sun on 2016/12/14.
//  Copyright © 2016年 Smobiler. All rights reserved.
//

#include "utils_impl.hpp"
#include "utils_interface.hpp"

namespace smobiler {
    
    
    static std::shared_ptr<UtilsInterface> m_utils;
    
    void Utils::create(const std::shared_ptr<UtilsInterface> &interface)
    {
        m_utils = interface;
    }
    
    std::string UtilsImpl::getRootpath()
    {
        if(m_utils) return m_utils->getRootPath();
        
        return "";
    }
}
