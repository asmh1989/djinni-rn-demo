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
#include "log_interface.hpp"
#include <string>

#define TAG_SOCKET      "socket"
#define TAG_SETTING     "setting"
#define TAG_ERROR       "error"
#define TAG_AUDIO       "audio"
#define TAG_IMAGE       "image"
#define TAG_VIEW        "view"
#define TAG_API         "api"
#define TAG_OFFLINE     "office"

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
