//
//  utils_impl.hpp
//  StmlCpp
//
//  Created by sun on 2016/12/14.
//  Copyright © 2016年 Smobiler. All rights reserved.
//

#ifndef utils_impl_hpp
#define utils_impl_hpp

#include "utils.hpp"
#include "utils_interface.hpp"
#include <string>

namespace smobiler {
    class UtilsImpl : public Utils
    {
    public:
        static std::string getRootpath();
    };
}
#endif /* utils_impl_hpp */
