#include "event_loop.h"
#include "channel.h"
#include "poller.h"
#include "waken_channel.h"
#include "event_timer_queue.h"
#include <cassert>

CYZPP_BEGIN

thread_local EventLoop *event_loop_this_thread = nullptr;

EventLoop::EventLoop()
    : quit_(false),
      looping_thread_id_(std::this_thread::get_id()),
      is_looping_(false),
      poller_(std::make_unique<Poller>(this)),
      event_timer_queue_(std::make_unique<EventTimerQueue>(this)),
      wake_channel_(std::make_unique<WakenChannel>(this)) {
  if (event_loop_this_thread != nullptr) {
    ERRLOG << "fatal error in event loop: one loop per thread\n";
    std::terminate();
  }
  // set thread_local varibale
  event_loop_this_thread = this;
}

EventLoop::~EventLoop() {
  assert(!is_looping_);
  // recover the thread_local variable
  event_loop_this_thread = nullptr;
}

// infinite loop
void EventLoop::loop() {
  // must run in looping thread
  assertIsLoopingThread();
  assert(!is_looping_);
  quit_ = false;
  is_looping_ = true;
  while (!quit_) {
    // remove old active channel
    active_channel_.clear();
    poller_->poll(-1, active_channel_);
    // handle all active events
    for (auto &&channel : active_channel_) {
      channel->handleEvents();
    }
    wake_channel_->runPendingEvent();
  }
  is_looping_ = false;
}

void EventLoop::runAt(const Function &call, TimePoint when) {
  event_timer_queue_->addEventTimer(call, when, TimeDuration{});
}

void EventLoop::runAfter(const Function &call, TimeDuration interval) {
  event_timer_queue_->addEventTimer(call, Clock::now() + interval, TimeDuration{});
}

void EventLoop::runInLoop(const Function &func) {
  // at IO thread, just call
  if(isLoopingThread())
    func();
  else {
    queueInLoop(func);
  }
}

void EventLoop::queueInLoop(const Function &func) {
  wake_channel_->addPendingEvent(func);
  if(!isLoopingThread() || wake_channel_->isRunningPendingEvent())
    wakeup();
}

void EventLoop::wakeup() {
  wake_channel_->wakeup();
}

// stop loop
void EventLoop::quit() { 
  quit_ = true;
  if(!isLoopingThread())
    wakeup();
}

// update Channel state in poller
void EventLoop::updateChannel(Channel &channel) {
  assertIsLoopingThread();
  assert(poller_->getOwnerLoop() == this);
  poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel &channel) {
  assertIsLoopingThread();
  assert(poller_->getOwnerLoop() == this);
  poller_->removeChannel(channel);
}

// get the event loop of this thread
EventLoop* EventLoop::getEventLoopThisThread() {
    return event_loop_this_thread;
}

// check is looping thread
void EventLoop::assertIsLoopingThread() {
  if(!isLoopingThread()) {
    ERRLOG << "fatal error in event_loop: did not run in looping thread\n";
    std::terminate();
  }
}

bool EventLoop::isLoopingThread() {
  return std::this_thread::get_id() == looping_thread_id_;
}

CYZPP_END
