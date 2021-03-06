// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from event_loop.djinni

#include "NativeEventLoop.hpp"  // my header
#include "NativeAsyncTask.hpp"

namespace djinni_generated {

NativeEventLoop::NativeEventLoop() : ::djinni::JniInterface<::smobiler::EventLoop, NativeEventLoop>() {}

NativeEventLoop::~NativeEventLoop() = default;

NativeEventLoop::JavaProxy::JavaProxy(JniType j) : Handle(::djinni::jniGetThreadEnv(), j) { }

NativeEventLoop::JavaProxy::~JavaProxy() = default;

void NativeEventLoop::JavaProxy::post(const std::shared_ptr<::smobiler::AsyncTask> & c_task) {
    auto jniEnv = ::djinni::jniGetThreadEnv();
    ::djinni::JniLocalScope jscope(jniEnv, 10);
    const auto& data = ::djinni::JniClass<::djinni_generated::NativeEventLoop>::get();
    jniEnv->CallVoidMethod(Handle::get().get(), data.method_post,
                           ::djinni::get(::djinni_generated::NativeAsyncTask::fromCpp(jniEnv, c_task)));
    ::djinni::jniExceptionCheck(jniEnv);
}

}  // namespace djinni_generated
