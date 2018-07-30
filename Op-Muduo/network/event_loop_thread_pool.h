#ifndef _CYZPP_EVENT_LOOP_THREAD_POOL_H__
#define _CYZPP_EVENT_LOOP_THREAD_POOL_H__

#include "../common.h"
#include <vector>
#include <thread>

CYZPP_BEGIN

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool {
 public:
  EventLoopThreadPool(EventLoop* loop, size_t thread_num);

  EventLoopThreadPool(const EventLoopThreadPool&) = delete;

  EventLoopThreadPool& operator=(const EventLoopThreadPool&) = delete;

  ~EventLoopThreadPool();

  void start();

  EventLoop* getLoop();

 private:
  std::vector<std::thread> thread_pool_;

  std::vector<EventLoop*> event_loop_pool_;

  std::vector<EventLoop*>::iterator current_loop_iter_;
};

CYZPP_END



#endif // !_CYZPP_EVENT_LOOP_THREAD_POOL_H__