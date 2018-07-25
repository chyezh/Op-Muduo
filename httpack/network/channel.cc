#include "channel.h"
#include <sys/epoll.h>
#include "event_loop.h"

CYZPP_BEGIN

Channel::Channel(SocketFD fd, EventLoop *loop)
    : status_(Status::NO_POLLER),
      sockfd_(fd),
      event_loop_(loop),
      events_(0),
      revents_(0) {}

void Channel::enableRead(bool on) {
  if (on) {
    events_ |= EPOLLIN | EPOLLPRI;
  } else {
    events_ &= ~(EPOLLIN | EPOLLPRI);
  }
  updateState();
}

void Channel::enableWrite(bool on) {
  if (on) {
    events_ |= EPOLLOUT;
  } else {
    events_ &= ~EPOLLOUT;
  }
  updateState();
}

void Channel::handleEvents() {
  if ((revents_ & EPOLLERR) && error_call_) error_call_();
  if ((revents_ & (EPOLLIN | EPOLLPRI)) && read_call_) read_call_();
  if ((revents_ & EPOLLOUT) && write_call_) write_call_();
}

void Channel::updateState() { event_loop_->updateChannel(this); }

CYZPP_END
