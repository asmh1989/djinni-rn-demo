//
//  log_impl.cpp
//  StmlCpp
//
//  Created by sun on 2016/12/14.
//  Copyright © 2016年 Smobiler. All rights reserved.
//

#include "log_impl.hpp"

namespace smobiler {
    static std::shared_ptr<LogInterface> m_log;
    static shared_ptr<smobiler::SingleThreadTaskRunner> m_runner;
    
    void LogImpl::set(shared_ptr<smobiler::LogInterface> log_impl, const shared_ptr<smobiler::SingleThreadTaskRunner> &runner)
    {
        m_log = log_impl;
        m_runner = runner;
    }
    
    void LogImpl::d(const std::string &tag, const std::string &msg)
    {
        if(m_log){
            
            m_runner->post([tag, msg](){
                m_log->d(tag, msg);
            });
            
        }
    }
    
    void LogImpl::w(const std::string &tag, const std::string &msg)
    {
        if(m_log){
            m_runner->post([tag, msg](){
                m_log->w(tag, msg);
            });
        }
    }
    
    void LogImpl::e(const std::string &tag, const std::string &msg)
    {
        if(m_log){
            m_runner->post([tag, msg](){
                m_log->e(tag, msg);
            });
        }
    }
    
    void LogImpl::i(const std::string &tag, const std::string &msg)
    {
        if(m_log){
            m_runner->post([tag, msg](){
                m_log->i(tag, msg);
            });
        }
    }
}
