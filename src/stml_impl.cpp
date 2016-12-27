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
#include "json11.hpp"
#include "event_loop_impl.hpp"
#include "event_loop.hpp"
#include "thread_launcher.hpp"

namespace smobiler {
    std::shared_ptr<Stml> Stml::create(const std::shared_ptr<LogInterface> &log, const std::shared_ptr<Cache> &cache, const std::shared_ptr<EventLoop> &ui_thread, const std::shared_ptr<ThreadLauncher> &launcher)
    {
        const auto ui_runner = make_shared<EventLoopRef>(ui_thread);
        const auto bg_runner = make_shared<EventLoopCpp>(launcher);
        return std::make_shared<StmlImpl>(log, cache, ui_runner, bg_runner);
    }
    
    StmlImpl::StmlImpl(const std::shared_ptr<LogInterface> &log,
                       const std::shared_ptr<Cache> &cache,
                       const shared_ptr<SingleThreadTaskRunner>&ui_runner,
                       const shared_ptr<SingleThreadTaskRunner> &bg_runner
                       ) :
    m_ui_thread {ui_runner},
    m_bg_thread {bg_runner}
    {
        m_listener = nullptr;
        
        LogImpl::set(log, bg_runner);
        
        
        Dogrobber::getInstance().setEventCallback(bind(&StmlImpl::onDogrobberEvent,
                                                       this,
                                                       std::placeholders::_1,
                                                       std::placeholders::_2,
                                                       std::placeholders::_3));
    }
    
    void StmlImpl::start(const std::string &server, int32_t port)
    {
        if(!Dogrobber::getInstance().isConnected()){
            Dogrobber::getInstance().setPeer(server,port);
            Dogrobber::getInstance().hello();
        } else {
            if(m_listener) m_listener->connected();
        }
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
    
    void StmlImpl::setTimeout(int32_t timeout)
    {
        
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
                if(m_listener){
                    m_bg_thread->post([&](){
                        m_listener->connected();
                    });
                }
                break;
            }
            case Dogrobber::EventType::EReconnected:
            {
                break;
            }
            case Dogrobber::EventType::EDisconnect:
            {
                if(m_listener){
                    
                    std::string errmsg="";
                    if(error){
                        errmsg = error.message();
                    }
                    
                    m_bg_thread->post([&, errmsg](){
                        m_listener->disconnect(errmsg);
                    });
                }
                break;
            }
            case Dogrobber::EventType::EReceived:
            {
                
                if(error)
                {
                    LOGE(TAG_SOCKET, error.message());
                    break;
                }
                
                
                switch(pResponse->contentType)
                {
                    
                    case ContentType::STML:
                    {
                        auto xml = move(pResponse->xmlDoc);
//                        XMLPrinter print;
//                        xml->Accept(&print);
                        
//                        stringstream sst;
//                        sst << "received: " << print.CStr();
                        
//                        LogImpl::d(TAG_SOCKET, sst.str());
                        
                        if(!m_listener) break;
                        
                        XMLElement* root = xml->RootElement();

                        XMLElement* entity = root->FirstChildElement();

                        json11::Json::array jsonarray;
                        
                        {
                            json11::Json::object obj;
                            obj["Type"] = "start";
                            json11::Json json(obj);

                            jsonarray.push_back(obj);
                        }

                        while(entity){
                            std::string name = entity->Name();

                            if(name == "CREATE"){
                                auto attr = entity->FirstAttribute();
                                
                                json11::Json::object obj;
                                while(attr){
                                    
                                    obj[attr->Name()] = attr->Value();
                                    attr = attr->Next();
                                }
                                
                                json11::Json json(obj);
                                jsonarray.push_back(obj);

                            } else if(name== "Session"){
                                
                                json11::Json::object obj;
                                obj["Type"] = "end";
                                json11::Json json(obj);
                                jsonarray.push_back(obj);
                            }

                            entity = entity->NextSiblingElement();
                        }

                        std::string str;

                        json11::Json::object rootObj;
                        
                        rootObj["msg"] = jsonarray;
                        json11::Json rootJson(rootObj);

                        rootJson.dump(str);

                        if(!str.empty()){
                            m_bg_thread->post([&, str](){
                                m_listener->received(str);
                            });
                        }
                        
                        if(m_listener){
//                            m_listener->received(print.CStr());
                        }
                        
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
                    m_bg_thread->post([&](){
                        m_listener->sended("");
                    });
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
