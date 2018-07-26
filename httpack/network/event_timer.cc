#include "event_timer.h"

CYZPP_BEGIN

// if timer can cycle, restart it
void EventTimer::restart() {
  if (cycle_ == true) {
    expired_ = Clock::now();
    expired_ += interval_;
  } else
    expired_ = invalid_time_point;
}

CYZPP_END