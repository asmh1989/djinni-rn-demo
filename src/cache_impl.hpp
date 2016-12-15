//
//  cache_impl.hpp
//  StmlCpp
//
//  Created by sun on 2016/12/14.
//  Copyright © 2016年 Smobiler. All rights reserved.
//

#ifndef cache_impl_hpp
#define cache_impl_hpp

#include "cache.hpp"

namespace smobiler {
    class CacheImpl : public Cache
    {
    public:        
        virtual std::string get(const std::string & key, const std::string & defaultVaue);
        virtual bool find(const std::string & key);
        virtual bool del(const std::string & key);
        virtual bool put(const std::string & key, const std::string & value);
        
    };
}
#endif /* cache_impl_hpp */
