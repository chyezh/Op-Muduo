#ifndef _WAKEN_CHANNEL_H__
#define _WAKEN_CHANNEL_H__

#include "../common.h"
#include <functional>
#include <vector>
#include <memory>
#include <mutex>

CYZPP_BEGIN

class Channel;
class EventLoop;

class WakenChannel {
 public:
  typedef std::function<void()> PendingFunction;

  WakenChannel(EventLoop* loop);

  WakenChannel(const WakenChannel&) = delete;

  WakenChannel &operator=(const WakenChannel&) = delete;

  ~WakenChannel();

  void addPendingEvent(const PendingFunction& function);

  void wakeup();

  void runPendingEvent();

  bool isRunningPendingEvent() { return is_running_pending_event_; }

 private:
  void wakeCallback();

  // eventfd
  int eventfd_;

  bool is_running_pending_event_;

  // owner event loop
  EventLoop* owner_event_loop_;

  // saved pending event
  std::vector<PendingFunction> pending_event_;

  // mutex for protect pending event
  std::mutex pending_event_mutex_;

  // channel for polling the eventfd
  std::unique_ptr<Channel> channel_;
};

CYZPP_END

#endif // !_WAKEN_CHANNEL_H__
