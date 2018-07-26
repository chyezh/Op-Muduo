#ifndef _CYZPP_EVENT_TIMER_H__
#define _CYZPP_EVENT_TIMER_H__

#include <chrono>
#include <functional>
#include <atomic>
#include "../common.h"

CYZPP_BEGIN

class EventTimer {
 public:
  typedef std::function<void()> EventCallback;
  typedef std::chrono::steady_clock Clock;
  typedef typename Clock::time_point TimePoint;
  typedef typename Clock::duration TimeDuration;
  typedef uint64_t EventTimerID;

  static const TimePoint invalid_time_point;

  EventTimer(const EventCallback &call, TimePoint when, TimeDuration interval)
      : expired_(when), interval_(interval), callback_(call), cycle_(false) {}

  TimePoint getExpiration() const { return expired_; }

  void enableCycle(bool on) { cycle_ = on; }

  void run() const { callback_(); }

  void restart();

  // generate unique id for timer
  static EventTimerID generateID();

 private:
  // expired time point
  TimePoint expired_;
  // duration
  TimeDuration interval_;
  // callback
  EventCallback callback_;
  // cycle
  bool cycle_;
  // unique id generator
  static std::atomic_uint64_t id_generator;
};

CYZPP_END

#endif  // !_CYZPP_TIMER_H__