#include "channel.h"
#include "event_loop.h"
#include <sys/epoll.h>

CYZPP_BEGIN

void Channel::enableRead(bool on) {
  if(on) {
    events_ |= EPOLLIN | EPOLLPRI;
  } else {
    events_ &= ~(EPOLLIN | EPOLLPRI);
  }
  updateState();
}

void Channel::enableWrite(bool on) {
  if(on) {
      events_ |= EPOLLOUT;
  } else {
      events_ &= ~EPOLLOUT;
  }
  updateState();
}

void Channel::handleEvents() { 
  if((revent_ & EPOLLERR) && error_call_)
    error_call_();
  if((revent_ & (EPOLLIN | EPOLLPRI)) && read_call_)
    read_call_();
  if((revent_ & EPOLLOUT) && write_call_)
    write_call_();
}

void Channel::updateState() {
  event_loop_->updateChannel(this);
}

CYZPP_END

