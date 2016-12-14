//
//  log_impl.hpp
//  StmlCpp
//
//  Created by sun on 2016/12/14.
//  Copyright © 2016年 Smobiler. All rights reserved.
//

#ifndef log_impl_hpp
#define log_impl_hpp

#include "log.hpp"
#include <string>

namespace smobiler {
    class LogImpl : public smobiler::Log {
    public:
        static void d(const std::string & tag, const std::string & msg);
        
        static void w(const std::string & tag, const std::string & msg);
        
        static void e(const std::string & tag, const std::string & msg);
        
        static void i(const std::string & tag, const std::string & msg);
        
    };
    
}
#endif /* log_impl_hpp */
