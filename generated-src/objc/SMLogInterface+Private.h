// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from stml.djinni

#include "log_interface.hpp"
#include <memory>

static_assert(__has_feature(objc_arc), "Djinni requires ARC to be enabled for this file");

@protocol SMLogInterface;

namespace djinni_generated {

class LogInterface
{
public:
    using CppType = std::shared_ptr<::smobiler::LogInterface>;
    using CppOptType = std::shared_ptr<::smobiler::LogInterface>;
    using ObjcType = id<SMLogInterface>;

    using Boxed = LogInterface;

    static CppType toCpp(ObjcType objc);
    static ObjcType fromCppOpt(const CppOptType& cpp);
    static ObjcType fromCpp(const CppType& cpp) { return fromCppOpt(cpp); }

private:
    class ObjcProxy;
};

}  // namespace djinni_generated

