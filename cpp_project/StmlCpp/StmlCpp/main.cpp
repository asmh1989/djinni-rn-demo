//
//  main.cpp
//  StmlCpp
//
//  Created by sun on 2016/12/14.
//  Copyright © 2016年 Smobiler. All rights reserved.
//

#include <iostream>
#include "log_impl.hpp"
#include "stml_impl.hpp"

using namespace smobiler;

std::shared_ptr<Stml> _stml;

class LogHandler : public smobiler::LogInterface
{
public:
    virtual void d(const std::string & tag, const std::string & msg)
    {
        printf("DEBUG: %s: %s\n", tag.c_str(), msg.c_str());
    }
    
    virtual void w(const std::string & tag, const std::string & msg)
    {
        printf("WRAN: %s: %s\n", tag.c_str(), msg.c_str());

    }
    
    virtual void e(const std::string & tag, const std::string & msg)
    {
        printf("ERROR: %s: %s\n", tag.c_str(), msg.c_str());

    }
    
    virtual void i(const std::string & tag, const std::string & msg)
    {
        printf("INFO: %s: %s\n", tag.c_str(), msg.c_str());
    }
};

class MyStml : public StmlListener
{
public:
    
    virtual void connecting()
    {
        LogImpl::d(TAG_SOCKET, "connecting...");
    }
    
    virtual void connected()
    {
    
        LogImpl::d(TAG_SOCKET, "connected...");
        
        _stml->post("<Session ID=\"test_mac\"><Connect User=\"chiyu\" Mode=\"Request\" Form=\"\" DeviceOS=\"IOS\" DeviceOSVersion=\"10.1.1\" DeviceVersion=\"3.5.0\" Version=\"0.0.1\" Device=\"iPhone7,1\" ServiceKey=\"\"/></Session>", false);
    }
    
    virtual void disconnect(const std::string & error)
    {
        LogImpl::e(TAG_SOCKET, error);

    }
    
    virtual void received(const std::string & msg)
    {
        LogImpl::d(TAG_SOCKET, "received...");

    }
    
    virtual void sended(const std::string & msg)
    {
        LogImpl::d(TAG_SOCKET, "sended...");
    }
    
    virtual void sending(const std::string & msg, bool showloading)
    {
        LogImpl::d(TAG_SOCKET, "sending...");
    }
    
private:
    
};

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    auto handler = std::make_shared<LogHandler>();
    Log::create(handler);
    
//    LogImpl::d("test", "test1");
//    LogImpl::w("test", "test1");
//    LogImpl::e("test", "test1");
//    LogImpl::i("test", "test1");
    
    _stml =  StmlImpl::create();
    _stml->setListener(std::make_shared<MyStml>());
    
    
    _stml->start("192.168.7.59", 23);
    
    while(true){
        sleep(500);
    }
    
    return 0;
}
