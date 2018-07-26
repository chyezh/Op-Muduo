#ifndef _EVENT_TIMER_QUEUE_H__
#define _EVENT_TIMER_QUEUE_H__
#include <map>
#include <sys/timerfd.h>
#include <memory>
#include "event_timer.h"
#include <vector>

CYZPP_BEGIN

class EventLoop;

class Channel;

class EventTimerQueue {
 public:
  typedef typename EventTimer::Clock Clock;
  typedef typename EventTimer::TimePoint TimePoint;
  typedef typename EventTimer::EventTimerID EventTimerID;
  typedef std::pair<TimePoint, EventTimerID> EventTimerKey;
  typedef std::map<EventTimerKey, EventTimer> EventTimerMap;

  EventTimerQueue(EventLoop* loop);

  EventTimerQueue(const EventTimerQueue&) = delete;

  EventTimerQueue &operator=(const EventTimerQueue&) = delete;

  void addEventTimer(const EventTimer& timer);

  void runner();

 private:
  std::vector<EventTimer> getExpiredTimer();

  void updateTimerFD(TimePoint new_time);

  // onwer event loop
  EventLoop* owner_event_loop_;
  // record event timer
  EventTimerMap event_timer_map_;
  // timerfd
  int timerfd_;
  // Channel as dispatcher
  std::unique_ptr<Channel> timer_channel_;
};

CYZPP_END

#endif // !_EVENT_TIMER_QUEUE_H__
