//
//  stml_impl.cpp
//  StmlCpp
//
//  Created by sun on 2016/12/14.
//  Copyright © 2016年 Smobiler. All rights reserved.
//

#include "stml_impl.hpp"

namespace smobiler {
    std::shared_ptr<Stml> Stml::create()
    {
        return std::make_shared<StmlImpl>();
    }
    
    StmlImpl::StmlImpl()
    {
        m_listener = nullptr;
    }
    
    void StmlImpl::start(const std::string &server, int32_t port)
    {
        
    }
    
    void StmlImpl::setKeeplive(bool alive)
    {
        
    }
    
    void StmlImpl::forceReconnect()
    {
        
    }
    
    void StmlImpl::post(const std::string &msg, bool showloading)
    {
        
    }
    
    void StmlImpl::bye()
    {
        
    }
    
    void StmlImpl::setListener(const std::shared_ptr<StmlListener> &listener)
    {
        m_listener = listener;
    }
}
