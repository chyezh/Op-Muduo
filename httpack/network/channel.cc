#include "channel.h"
#include <sys/epoll.h>
#include <cassert>
#include "event_loop.h"

CYZPP_BEGIN

Channel::Channel(SocketFD fd, EventLoop *loop)
    : events_(0),
      revents_(0),
      sockfd_(fd),
      owner_event_loop_(loop),
      status_(Status::NO_POLLER),
      is_handling_(false) {}

Channel::~Channel() {
  assert(!is_handling_);
  if (status_ != Status::NO_POLLER) removeSelf();
  assert(status_ == Status::NO_POLLER);
}

bool Channel::isReading() {
  return events_ & (EPOLLIN | EPOLLPRI);
}

bool Channel::isWriting() {
  return events_ & (EPOLLOUT);
}

void Channel::enableRead(bool on) {
  owner_event_loop_->assertIsLoopingThread();
  if (on) {
    events_ |= EPOLLIN | EPOLLPRI;
  } else {
    events_ &= ~(EPOLLIN | EPOLLPRI);
  }
  updateState();
}

void Channel::enableWrite(bool on) {
  owner_event_loop_->assertIsLoopingThread();
  if (on) {
    events_ |= EPOLLOUT;
  } else {
    events_ &= ~EPOLLOUT;
  }
  updateState();
}

void Channel::removeSelf() {
  owner_event_loop_->assertIsLoopingThread();
  owner_event_loop_->removeChannel(*this);
}

void Channel::handleEvents() {
  owner_event_loop_->assertIsLoopingThread();
  is_handling_ = true;
  if ((revents_ & EPOLLERR) && error_call_) error_call_();
  if ((revents_ & (EPOLLIN | EPOLLPRI)) && read_call_) read_call_();
  if ((revents_ & EPOLLOUT) && write_call_) write_call_();
  is_handling_ = false;
}

void Channel::updateState() { owner_event_loop_->updateChannel(*this); }

CYZPP_END
