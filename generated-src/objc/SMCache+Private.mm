// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from stml.djinni

#import "SMCache+Private.h"
#import "SMCache.h"
#import "DJICppWrapperCache+Private.h"
#import "DJIError.h"
#import "DJIMarshal+Private.h"
#include <exception>
#include <stdexcept>
#include <utility>

static_assert(__has_feature(objc_arc), "Djinni requires ARC to be enabled for this file");

@interface SMCache ()

- (id)initWithCpp:(const std::shared_ptr<::smobiler::Cache>&)cppRef;

@end

@implementation SMCache {
    ::djinni::CppProxyCache::Handle<std::shared_ptr<::smobiler::Cache>> _cppRefHandle;
}

- (id)initWithCpp:(const std::shared_ptr<::smobiler::Cache>&)cppRef
{
    if (self = [super init]) {
        _cppRefHandle.assign(cppRef);
    }
    return self;
}

- (nonnull NSString *)get:(nonnull NSString *)key
              defaultVaue:(nonnull NSString *)defaultVaue {
    try {
        auto objcpp_result_ = _cppRefHandle.get()->get(::djinni::String::toCpp(key),
                                                       ::djinni::String::toCpp(defaultVaue));
        return ::djinni::String::fromCpp(objcpp_result_);
    } DJINNI_TRANSLATE_EXCEPTIONS()
}

- (BOOL)find:(nonnull NSString *)key {
    try {
        auto objcpp_result_ = _cppRefHandle.get()->find(::djinni::String::toCpp(key));
        return ::djinni::Bool::fromCpp(objcpp_result_);
    } DJINNI_TRANSLATE_EXCEPTIONS()
}

- (BOOL)del:(nonnull NSString *)key {
    try {
        auto objcpp_result_ = _cppRefHandle.get()->del(::djinni::String::toCpp(key));
        return ::djinni::Bool::fromCpp(objcpp_result_);
    } DJINNI_TRANSLATE_EXCEPTIONS()
}

- (BOOL)put:(nonnull NSString *)key
      value:(nonnull NSString *)value {
    try {
        auto objcpp_result_ = _cppRefHandle.get()->put(::djinni::String::toCpp(key),
                                                       ::djinni::String::toCpp(value));
        return ::djinni::Bool::fromCpp(objcpp_result_);
    } DJINNI_TRANSLATE_EXCEPTIONS()
}

- (nonnull NSString *)getRootPath {
    try {
        auto objcpp_result_ = _cppRefHandle.get()->getRootPath();
        return ::djinni::String::fromCpp(objcpp_result_);
    } DJINNI_TRANSLATE_EXCEPTIONS()
}

namespace djinni_generated {

auto Cache::toCpp(ObjcType objc) -> CppType
{
    if (!objc) {
        return nullptr;
    }
    return objc->_cppRefHandle.get();
}

auto Cache::fromCppOpt(const CppOptType& cpp) -> ObjcType
{
    if (!cpp) {
        return nil;
    }
    return ::djinni::get_cpp_proxy<SMCache>(cpp);
}

}  // namespace djinni_generated

@end
