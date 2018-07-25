#include "poller.h"
#include <stdio.h>
#include <cassert>
#include <cstring>
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
      return;
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

void Poller::updateChannel(Channel &channel) {
  Channel::Status status = channel.getStatus();
  // if channel do not have poller, add to this poller
  if (status == Channel::Status::NO_POLLER) {
    // add to poller
    channel_list_[channel.getSockfd()] = &channel;
    // if channel have interesting events
    if (!channel.getEvents()) {
      // add to epoll
      update(EPOLL_CTL_ADD, channel);
      channel.setStatus(Channel::Status::POLLING);
    } else {
      // not add to epoll
      channel.setStatus(Channel::Status::IN_POLLER);
    }
    return;
    // if channel has poller, update the status of channel
  } else {
    // if channel have interesting events
    if (channel.getEvents()) {
      // if polling, modify
      if (status == Channel::Status::POLLING) update(EPOLL_CTL_MOD, channel);
      // if in poll but not polling, add
      else
        update(EPOLL_CTL_ADD, channel);
      // set status to polling
      channel.setStatus(Channel::Status::POLLING);
      // if channel has no interesting events, remove it from epoll
    } else {
      update(EPOLL_CTL_DEL, channel);
      channel.setStatus(Channel::Status::IN_POLLER);
    }
  }
}

void Poller::removeChannel(Channel &channel) {
  Channel::Status status = channel.getStatus();
  // channel must be control by this poller
  assert(status != Channel::Status::NO_POLLER);
  // if channel at polling state, remove it from epoll
  if (status == Channel::Status::POLLING) update(EPOLL_CTL_DEL, channel);
  channel.setStatus(Channel::Status::NO_POLLER);
  // remove from this poller
  channel_list_.erase(channel.getSockfd());
}

bool Poller::hasChannel(const Channel &channel) {
  return channel_list_.find(channel.getSockfd()) != channel_list_.end();
}

void Poller::update(int op, Channel &channel) {
  struct epoll_event new_event;
  bzero(&new_event, sizeof(epoll_event));
  new_event.data.ptr = static_cast<void *>(&channel);
  new_event.events = channel.getEvents();
  ::epoll_ctl(poll_id_, op, channel.getSockfd(), &new_event);
}

CYZPP_END
