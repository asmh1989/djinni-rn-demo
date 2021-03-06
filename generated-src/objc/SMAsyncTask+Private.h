// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from event_loop.djinni

#include "async_task.hpp"
#include <memory>

static_assert(__has_feature(objc_arc), "Djinni requires ARC to be enabled for this file");

@class SMAsyncTask;

namespace djinni_generated {

class AsyncTask
{
public:
    using CppType = std::shared_ptr<::smobiler::AsyncTask>;
    using CppOptType = std::shared_ptr<::smobiler::AsyncTask>;
    using ObjcType = SMAsyncTask*;

    using Boxed = AsyncTask;

    static CppType toCpp(ObjcType objc);
    static ObjcType fromCppOpt(const CppOptType& cpp);
    static ObjcType fromCpp(const CppType& cpp) { return fromCppOpt(cpp); }

private:
    class ObjcProxy;
};

}  // namespace djinni_generated

