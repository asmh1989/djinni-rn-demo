// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from stml.djinni

#import "SMLogInterface+Private.h"
#import "SMLogInterface.h"
#import "DJIMarshal+Private.h"
#import "DJIObjcWrapperCache+Private.h"
#include <stdexcept>

static_assert(__has_feature(objc_arc), "Djinni requires ARC to be enabled for this file");

namespace djinni_generated {

class LogInterface::ObjcProxy final
: public ::smobiler::LogInterface
, public ::djinni::ObjcProxyCache::Handle<ObjcType>
{
public:
    using Handle::Handle;
    void d(const std::string & c_tag, const std::string & c_msg) override
    {
        @autoreleasepool {
            [Handle::get() d:(::djinni::String::fromCpp(c_tag))
                         msg:(::djinni::String::fromCpp(c_msg))];
        }
    }
    void w(const std::string & c_tag, const std::string & c_msg) override
    {
        @autoreleasepool {
            [Handle::get() w:(::djinni::String::fromCpp(c_tag))
                         msg:(::djinni::String::fromCpp(c_msg))];
        }
    }
    void e(const std::string & c_tag, const std::string & c_msg) override
    {
        @autoreleasepool {
            [Handle::get() e:(::djinni::String::fromCpp(c_tag))
                         msg:(::djinni::String::fromCpp(c_msg))];
        }
    }
    void i(const std::string & c_tag, const std::string & c_msg) override
    {
        @autoreleasepool {
            [Handle::get() i:(::djinni::String::fromCpp(c_tag))
                         msg:(::djinni::String::fromCpp(c_msg))];
        }
    }
};

}  // namespace djinni_generated

namespace djinni_generated {

auto LogInterface::toCpp(ObjcType objc) -> CppType
{
    if (!objc) {
        return nullptr;
    }
    return ::djinni::get_objc_proxy<ObjcProxy>(objc);
}

auto LogInterface::fromCppOpt(const CppOptType& cpp) -> ObjcType
{
    if (!cpp) {
        return nil;
    }
    return dynamic_cast<ObjcProxy&>(*cpp).Handle::get();
}

}  // namespace djinni_generated
