#include "event_timer.h"

CYZPP_BEGIN

std::atomic_uint64_t EventTimer::id_generator;

const EventTimer::TimePoint EventTimer::invalid_time_point = TimePoint::min();

EventTimer::EventTimerID EventTimer::generateID() { return id_generator.fetch_add(1); }

// if timer can cycle, restart it
void EventTimer::restart() {
  if (cycle_ == true) {
    expired_ = Clock::now();
    expired_ += interval_;
  } else
    expired_ = invalid_time_point;
}

CYZPP_END