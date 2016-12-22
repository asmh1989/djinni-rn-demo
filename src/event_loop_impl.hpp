#pragma once
#include "stl.hpp"
#include <thread>
#include <queue>
#include <atomic>

#include "event_loop.hpp"
#include "async_task.hpp"
#include "thread_launcher.hpp"
#include "single_thread_task_runner.hpp"

namespace smobiler {

// a helper class to turn the `AsyncTask` api into a std::function based one
class FnTask final : public AsyncTask {
public:
    FnTask(function<void()> run_me);
    virtual void execute() override;
private:
    function<void()> m_fn;
};

// an interface wrapper on top of the platform event loops
class EventLoopRef final : public SingleThreadTaskRunner {
  public:
    EventLoopRef(shared_ptr<EventLoop> loop);
    virtual void post(const SingleThreadTaskRunner::Task & task) override;
  private:
    const shared_ptr<EventLoop> m_loop;
};

// A implementation of implemented in pure c++.
class EventLoopCpp final : public SingleThreadTaskRunner {
  public:
    EventLoopCpp(const shared_ptr<ThreadLauncher> & launcher);
    ~EventLoopCpp();
    virtual void post(const SingleThreadTaskRunner::Task & task) override;

  private:
    // the actual run loop runs here
    void _run_loop();
    std::mutex m_task_mutex;
    std::condition_variable m_task_cv;
    bool m_stop;
    std::queue<SingleThreadTaskRunner::Task> m_queue;

    bool m_done;
    std::mutex m_done_mutex;
    std::condition_variable m_done_cv;
};

}
