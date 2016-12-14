//
//  log_impl.cpp
//  StmlCpp
//
//  Created by sun on 2016/12/14.
//  Copyright © 2016年 Smobiler. All rights reserved.
//

#include "log_impl.hpp"
#include "log_interface.hpp"

namespace smobiler {
    static std::shared_ptr<LogInterface> m_log;

    void Log::create(const std::shared_ptr<LogInterface> &l)
    {
        m_log = l;
    }
    
    
    void LogImpl::d(const std::string &tag, const std::string &msg)
    {
        if(m_log) m_log->d(tag, msg);
    }
    
    void LogImpl::w(const std::string &tag, const std::string &msg)
    {
        if(m_log) m_log->w(tag, msg);
    }
    
    void LogImpl::e(const std::string &tag, const std::string &msg)
    {
        if(m_log) m_log->e(tag, msg);
    }
    
    void LogImpl::i(const std::string &tag, const std::string &msg)
    {
        if(m_log) m_log->i(tag, msg);
    }
}
