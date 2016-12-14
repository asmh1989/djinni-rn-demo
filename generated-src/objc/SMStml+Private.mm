// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from stml.djinni

#import "SMStml+Private.h"
#import "SMStml.h"
#import "DJICppWrapperCache+Private.h"
#import "DJIError.h"
#import "DJIMarshal+Private.h"
#import "SMStmlListener+Private.h"
#include <exception>
#include <stdexcept>
#include <utility>

static_assert(__has_feature(objc_arc), "Djinni requires ARC to be enabled for this file");

@interface SMStml ()

- (id)initWithCpp:(const std::shared_ptr<::smobiler::Stml>&)cppRef;

@end

@implementation SMStml {
    ::djinni::CppProxyCache::Handle<std::shared_ptr<::smobiler::Stml>> _cppRefHandle;
}

- (id)initWithCpp:(const std::shared_ptr<::smobiler::Stml>&)cppRef
{
    if (self = [super init]) {
        _cppRefHandle.assign(cppRef);
    }
    return self;
}

- (void)start:(nonnull NSString *)server
         port:(int32_t)port {
    try {
        _cppRefHandle.get()->start(::djinni::String::toCpp(server),
                                   ::djinni::I32::toCpp(port));
    } DJINNI_TRANSLATE_EXCEPTIONS()
}

- (void)setKeeplive:(BOOL)alive {
    try {
        _cppRefHandle.get()->setKeeplive(::djinni::Bool::toCpp(alive));
    } DJINNI_TRANSLATE_EXCEPTIONS()
}

- (void)forceReconnect {
    try {
        _cppRefHandle.get()->forceReconnect();
    } DJINNI_TRANSLATE_EXCEPTIONS()
}

- (void)post:(nonnull NSString *)msg
 showloading:(BOOL)showloading {
    try {
        _cppRefHandle.get()->post(::djinni::String::toCpp(msg),
                                  ::djinni::Bool::toCpp(showloading));
    } DJINNI_TRANSLATE_EXCEPTIONS()
}

- (void)bye {
    try {
        _cppRefHandle.get()->bye();
    } DJINNI_TRANSLATE_EXCEPTIONS()
}

- (void)setListener:(nullable id<SMStmlListener>)listener {
    try {
        _cppRefHandle.get()->setListener(::djinni_generated::StmlListener::toCpp(listener));
    } DJINNI_TRANSLATE_EXCEPTIONS()
}

+ (nullable SMStml *)create {
    try {
        auto objcpp_result_ = ::smobiler::Stml::create();
        return ::djinni_generated::Stml::fromCpp(objcpp_result_);
    } DJINNI_TRANSLATE_EXCEPTIONS()
}

namespace djinni_generated {

auto Stml::toCpp(ObjcType objc) -> CppType
{
    if (!objc) {
        return nullptr;
    }
    return objc->_cppRefHandle.get();
}

auto Stml::fromCppOpt(const CppOptType& cpp) -> ObjcType
{
    if (!cpp) {
        return nil;
    }
    return ::djinni::get_cpp_proxy<SMStml>(cpp);
}

}  // namespace djinni_generated

@end
