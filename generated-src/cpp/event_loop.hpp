// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from event_loop.djinni

#pragma once

#include <memory>

namespace smobiler {

class AsyncTask;

class EventLoop {
public:
    virtual ~EventLoop() {}

    virtual void post(const std::shared_ptr<AsyncTask> & task) = 0;
};

}  // namespace smobiler
