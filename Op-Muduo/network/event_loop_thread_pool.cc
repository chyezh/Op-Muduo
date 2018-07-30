#include "event_loop_thread_pool.h"
#include "event_loop.h"
#include "event_loop_thread.h"

CYZPP_BEGIN

EventLoopThreadPool::EventLoopThreadPool(EventLoop *loop, size_t thread_num)
    : current_loop_iter_(nullptr) {
  // generate loop pool
  thread_pool_.resize(thread_num);
  event_loop_pool_.resize(thread_num + 1);
  event_loop_pool_[0] = loop;
  current_loop_iter_ = event_loop_pool_.begin();
}

EventLoopThreadPool::~EventLoopThreadPool() {
  for(auto &&event_loop : event_loop_pool_)
    event_loop->quit();
  for(auto &&thread : thread_pool_)
    thread.join();
}

void EventLoopThreadPool::start() {
  for(int i = 0; i < thread_pool_.size(); ++i)
    std::tie(thread_pool_[i], event_loop_pool_[i+1]) = EventLoopThread::makeEventLoop();
}

EventLoop* EventLoopThreadPool::getLoop() {
  auto return_loop_iter = current_loop_iter_;
  ++current_loop_iter_;
  if(current_loop_iter_ == event_loop_pool_.end())
    current_loop_iter_ = event_loop_pool_.begin();
  return *return_loop_iter;
}
CYZPP_END