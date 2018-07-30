#ifndef _CYZPP_EVENT_LOOP_THREAD_H__
#define _CYZPP_EVENT_LOOP_THREAD_H__

#include "../common.h"
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>

CYZPP_BEGIN

class EventLoop;

class EventLoopThread {
 public:
  EventLoopThread(const EventLoopThread &) = delete;

  EventLoopThread &operator=(const EventLoopThread &) = delete;

  static std::pair<std::thread, EventLoop*> makeEventLoop();

 private:
  static void makeEventLoopFunc();
  static EventLoop* event_loop_;
  static std::thread event_thread_;
  static std::mutex mutex_;
  static std::condition_variable cond_;
};

CYZPP_END

#endif // !_CYZPP_EVENT_LOOP_THREAD_H__