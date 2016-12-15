//
//  stml_impl.hpp
//  StmlCpp
//
//  Created by sun on 2016/12/14.
//  Copyright © 2016年 Smobiler. All rights reserved.
//

#ifndef stml_impl_hpp
#define stml_impl_hpp

#include "stml.hpp"
#include "stml_listener.hpp"
#include "sttp/dogrobber.h"

using namespace sttp;

namespace smobiler {
    class StmlImpl : public Stml
    {
    public:
        
        StmlImpl();
        
        virtual void start(const std::string & server, int32_t port);
        virtual void setKeeplive(bool alive);
        virtual void forceReconnect();
        virtual void post(const std::string & msg, bool showloading);
        virtual void bye();
        virtual void setListener(const std::shared_ptr<StmlListener> & listener);
        
    private:
        void onDogrobberEvent(Dogrobber::EventType,const error_code&,std::shared_ptr<SttpResponse>);
        void flushPost();
        
        vector<string> m_strDirectives;

        std::shared_ptr<StmlListener> m_listener;
    };
}

#endif /* stml_impl_hpp */
