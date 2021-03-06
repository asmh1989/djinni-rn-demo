// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from stml.djinni

#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace smobiler {

class Cache;
class EventLoop;
class LogInterface;
class StmlListener;
class ThreadLauncher;

/** stml */
class Stml {
public:
    virtual ~Stml() {}

    virtual void start(const std::string & server, int32_t port) = 0;

    virtual void setKeeplive(bool alive) = 0;

    virtual void setTimeout(int32_t timeout) = 0;

    virtual void forceReconnect() = 0;

    virtual void post(const std::string & msg, bool showloading) = 0;

    virtual void bye() = 0;

    virtual void setListener(const std::shared_ptr<StmlListener> & listener) = 0;

    static std::shared_ptr<Stml> create(const std::shared_ptr<LogInterface> & log, const std::shared_ptr<Cache> & cache, const std::shared_ptr<EventLoop> & ui_thread, const std::shared_ptr<ThreadLauncher> & launcher);
};

}  // namespace smobiler
