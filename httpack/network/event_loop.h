#ifndef _CYZPP_EVENT_LOOP_H__
#define _CYZPP_EVENT_LOOP_H__
#include <thread>
#include <atomic>
#include "poller.h"
#include <vector>

CYZPP_BEGIN

class EventLoop {
 private:
  typedef std::vector<Channel*> ChannelMap;

 public:
  EventLoop();

  void loop();

  void quit();

  void updateChannel(Channel* channel);

 private:

  std::atomic_bool quit_;
  Poller poller_;
  ChannelMap active_channel_;
};

thread_local EventLoop* event_loop_this_thread = nullptr;

CYZPP_END

#endif // !_CYZPP_EVENT_LOOP_H__
