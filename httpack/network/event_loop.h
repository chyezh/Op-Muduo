#ifndef _CYZPP_EVENT_LOOP_H__
#define _CYZPP_EVENT_LOOP_H__
#include <thread>
#include <atomic>
#include <vector>
#include <memory>
#include <functional>
#include "../common.h"

CYZPP_BEGIN

class Channel;

class Poller;

class EventTimerQueue;

class WakenChannel;

class EventLoop {
 private:
  typedef std::vector<Channel*> ChannelMap;
  typedef std::function<void()> Function;
  typedef std::chrono::steady_clock Clock;
  typedef typename Clock::time_point TimePoint;
  typedef typename Clock::duration TimeDuration;

 public:
  EventLoop();

  ~EventLoop();

  EventLoop(const EventLoop&) = delete;

  EventLoop &operator=(const EventLoop&) = delete;

  void loop();

  void runAt(const Function &call, TimePoint when);

  void runAfter(const Function &call, TimeDuration interval);

  void runInLoop(const Function &func);

  void queueInLoop(const Function &func);

  void wakeup();

  void quit();

  void updateChannel(Channel& channel);
  
  void removeChannel(Channel& channel);

  static EventLoop* getEventLoopThisThread();

  void assertIsLoopingThread();

  bool isLoopingThread();

 private:
  // can be controlled by other thread
  std::atomic_bool quit_;

  // looping thread id
  std::thread::id looping_thread_id_;
  
  // is looping is excute
  bool is_looping_;

  // poller per thread
  std::unique_ptr<Poller> poller_;

  // poll result
  ChannelMap active_channel_;

  // event_timer_queue
  std::unique_ptr<EventTimerQueue> event_timer_queue_;
  
  // waken channel
  std::unique_ptr<WakenChannel> wake_channel_;
};

CYZPP_END

#endif // !_CYZPP_EVENT_LOOP_H__
