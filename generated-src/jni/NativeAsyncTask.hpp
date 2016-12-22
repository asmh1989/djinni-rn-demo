// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from event_loop.djinni

#pragma once

#include "async_task.hpp"
#include "djinni_support.hpp"

namespace djinni_generated {

class NativeAsyncTask final : ::djinni::JniInterface<::smobiler::AsyncTask, NativeAsyncTask> {
public:
    using CppType = std::shared_ptr<::smobiler::AsyncTask>;
    using CppOptType = std::shared_ptr<::smobiler::AsyncTask>;
    using JniType = jobject;

    using Boxed = NativeAsyncTask;

    ~NativeAsyncTask();

    static CppType toCpp(JNIEnv* jniEnv, JniType j) { return ::djinni::JniClass<NativeAsyncTask>::get()._fromJava(jniEnv, j); }
    static ::djinni::LocalRef<JniType> fromCppOpt(JNIEnv* jniEnv, const CppOptType& c) { return {jniEnv, ::djinni::JniClass<NativeAsyncTask>::get()._toJava(jniEnv, c)}; }
    static ::djinni::LocalRef<JniType> fromCpp(JNIEnv* jniEnv, const CppType& c) { return fromCppOpt(jniEnv, c); }

private:
    NativeAsyncTask();
    friend ::djinni::JniClass<NativeAsyncTask>;
    friend ::djinni::JniInterface<::smobiler::AsyncTask, NativeAsyncTask>;

};

}  // namespace djinni_generated
