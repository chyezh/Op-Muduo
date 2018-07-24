#ifndef _CYZPP_POLLER_H__
#define _CYZPP_POLLER_H__

#include "common.h"
#include <sys/epoll.h>
#include <vector>
#include <map>

CYZPP_BEGIN

class Channel;
class EventLoop;

class Poller {
 public:
  typedef int PollID;
  typedef int SocketFD;
  typedef std::map<SocketFD, Channel*> ChannelMap;
  typedef std::vector<Channel*> ChannelList;
  typedef std::vector<struct epoll_event> EventList;

  Poller(EventLoop* loop);

  Poller(const Poller&) = delete;

  Poller &operator=(const Poller&) = delete;

  void poll(int timeout, ChannelList& poll_result_list);

  void updateChannel(const Channel &channel);

  void removeChannel(const Channel &channel);

  bool hasChannel(const Channel &channel);

 private:
  // auxiliary function

  // Channel controlled by this Poller
  ChannelMap channel_list_;
  // epoll return events
  EventList event_list_;

  EventLoop* event_loop_;



  PollID poll_id_;
};

CYZPP_END

#endif // !_CYZPP_POLLER_H__
