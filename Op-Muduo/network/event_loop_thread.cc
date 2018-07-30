#include "event_loop_thread.h"
#include "event_loop.h"

CYZPP_BEGIN

EventLoop *EventLoopThread::event_loop_ = nullptr;

std::thread EventLoopThread::event_thread_;

std::mutex EventLoopThread::mutex_;

std::condition_variable EventLoopThread::cond_;

std::pair<std::thread, EventLoop*> EventLoopThread::makeEventLoop() {
  event_thread_ = std::thread(makeEventLoopFunc);
  // wait event_loop_ return
  std::unique_lock<std::mutex> lk(mutex_);
  cond_.wait(lk, []() { return event_loop_ != nullptr; });
  // recover event_loop_
  EventLoop *new_event_loop = event_loop_;
  event_loop_ = nullptr;
  // detach the thread and return
  return std::make_pair(std::move(event_thread_), new_event_loop);
}

void EventLoopThread::makeEventLoopFunc() {
  EventLoop loop;
  {
    // return new event_loop_
    std::lock_guard<std::mutex> lk(mutex_);
    event_loop_ = &loop;
    cond_.notify_one();
  }
  // loop
  loop.loop();
}

CYZPP_END
