//
//  log_impl.hpp
//  StmlCpp
//
//  Created by sun on 2016/12/14.
//  Copyright © 2016年 Smobiler. All rights reserved.
//

#ifndef log_impl_hpp
#define log_impl_hpp

#include "log_interface.hpp"
#include <string>
#include "single_thread_task_runner.hpp"
#include "stl.hpp"

#define TAG_SOCKET      "socket"
#define TAG_SETTING     "setting"
#define TAG_ERROR       "error"
#define TAG_AUDIO       "audio"
#define TAG_IMAGE       "image"
#define TAG_VIEW        "view"
#define TAG_API         "api"
#define TAG_OFFLINE     "office"

#define LOGI(__TAG__, __MSG__) smobiler::LogImpl::instance()->i(__TAG__, __MSG__)
#define LOGD(__TAG__, __MSG__) smobiler::LogImpl::instance()->d(__TAG__, __MSG__)
#define LOGW(__TAG__, __MSG__) smobiler::LogImpl::instance()->w(__TAG__, __MSG__)
#define LOGE(__TAG__, __MSG__) smobiler::LogImpl::instance()->e(__TAG__, __MSG__)

namespace smobiler {
    
    class LogImpl {
    public:
        static LogImpl* instance(){
            static LogImpl _log;
            return &_log;
        }
        
        static void set(shared_ptr<LogInterface> log_impl, const shared_ptr<SingleThreadTaskRunner> & runner);

        void d(const std::string & tag, const std::string & msg);
        
        void w(const std::string & tag, const std::string & msg);
        
        void e(const std::string & tag, const std::string & msg);
        
        void i(const std::string & tag, const std::string & msg);
        
    };
    
}
#endif /* log_impl_hpp */
