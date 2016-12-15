//
//  stml_impl.cpp
//  StmlCpp
//
//  Created by sun on 2016/12/14.
//  Copyright © 2016年 Smobiler. All rights reserved.
//

#include "stml_impl.hpp"
#include "log_impl.hpp"
#include "parser/stml-parser.h"

namespace smobiler {
    std::shared_ptr<Stml> Stml::create()
    {
        return std::make_shared<StmlImpl>();
    }
    
    StmlImpl::StmlImpl()
    {
        m_listener = nullptr;
        
        Dogrobber::getInstance().setEventCallback(bind(&StmlImpl::onDogrobberEvent,
                                                       this,
                                                       std::placeholders::_1,
                                                       std::placeholders::_2,
                                                       std::placeholders::_3));
    }
    
    void StmlImpl::start(const std::string &server, int32_t port)
    {
        Dogrobber::getInstance().setPeer(server,port);
        Dogrobber::getInstance().hello();
    }
    
    void StmlImpl::setKeeplive(bool alive)
    {
        Dogrobber::getInstance().setKeepAlive(alive);
    }
    
    void StmlImpl::forceReconnect()
    {
        Dogrobber::getInstance().forceReconnect();

    }
    
    void StmlImpl::post(const std::string &msg, bool showloading)
    {
        m_strDirectives.push_back(msg);
        if(showloading){
            if(m_listener){
                m_listener->sending(msg, showloading);
            }
            
        }
        flushPost();
    }
    
    void StmlImpl::bye()
    {
        Dogrobber::getInstance().bye();
    }
    
    void StmlImpl::setListener(const std::shared_ptr<StmlListener> &listener)
    {
        m_listener = listener;
    }
    
    void StmlImpl::flushPost()
    {
        for(auto strDirective : m_strDirectives)
        {
            Dogrobber::getInstance().post(strDirective);
        }
        m_strDirectives.clear();
    }
    
    void StmlImpl::onDogrobberEvent(Dogrobber::EventType type, const error_code & error, std::shared_ptr<SttpResponse> pResponse)
    {
        switch(type)
        {
            case Dogrobber::EventType::EConnected:
            {
                if(m_listener) m_listener->connected();
                break;
            }
            case Dogrobber::EventType::EReconnected:
            {
                break;
            }
            case Dogrobber::EventType::EDisconnect:
            {
                if(m_listener){
                    if(error){
                        m_listener->disconnect(error.message());
                    } else {
                        m_listener->disconnect("");
                    }
                }
                break;
            }
            case Dogrobber::EventType::EReceived:
            {
                
                if(error)
                {
                    LogImpl::e(TAG_SOCKET, error.message());
                    break;
                }
                
                
                switch(pResponse->contentType)
                {
                    
                    case ContentType::STML:
                    {
                        auto xml = move(pResponse->xmlDoc);
                        XMLPrinter print;
                        xml->Accept(&print);
                        
                        stringstream sst;
                        sst << "received: " << print.CStr();
                        
                        LogImpl::d(TAG_SOCKET, sst.str());
                        
//                        STMLParser::Parser::instance().enqueue(move(pResponse->xmlDoc));
                        break;
                    }
                    default:
                    {
                        
                    }
                }
                break;
            }
            case Dogrobber::EventType::ESended:
            {
                if(m_listener){
                    m_listener->sended("");
                }
                break;
            }
            case Dogrobber::EventType::ETimeOut:
            {
                //if(m_tryReconnect) m_tryReconnect = false;
                break;
            }
            default:
            {
                //no default
            }
        }
    }
}
