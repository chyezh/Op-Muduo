#include "poller.h"
#include <stdio.h>
#include <exception>
#include "channel.h"

CYZPP_BEGIN

Poller::Poller(EventLoop *loop) : event_loop_(loop) {
  poll_id_ = epoll_create(0);
  if (poll_id_ < 0) {
    perror("Poller construct fail#");
    std::terminate();
  }
}

void Poller::poll(int timeout, ChannelList &poll_result_list) {
  int events_num =
      ::epoll_wait(poll_id_, event_list_.data(), event_list_.size(), timeout);
  if (events_num < 0) {
    if (errno != EINTR) {
      perror("Poller::poll error#");
    }
  }
  if (events_num > 0) {
    for (int i = 0; i < events_num; ++i) {
      Channel *channel_ptr = static_cast<Channel *>(event_list_[i].data.ptr);
      channel_ptr->setREvents(event_list_[i].events);
      poll_result_list.push_back(channel_ptr);
    }
    // if event list is full, grow up the space
    if (events_num == event_list_.size())
      event_list_.resize(2 * event_list_.size());
  }
}

void Poller::updateChannel(const Channel &channel) {
  
}

void Poller::removeChannel(const Channel &channel) {}

bool Poller::hasChannel(const Channel &channel) {}

CYZPP_END
