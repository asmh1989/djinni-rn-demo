//
//  main.cpp
//  StmlCpp
//
//  Created by sun on 2016/12/14.
//  Copyright © 2016年 Smobiler. All rights reserved.
//

#include <iostream>
#include "log_impl.hpp"

using namespace smobiler;

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


int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    auto handler = std::make_shared<LogHandler>();
    Log::create(handler);
    
    LogImpl::d("test", "test1");
    LogImpl::w("test", "test1");
    LogImpl::e("test", "test1");
    LogImpl::i("test", "test1");

    return 0;
}
