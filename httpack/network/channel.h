#ifndef _CYZPP_CHANNEL_H__
#define _CYZPP_CHANNEL_H__

#include "common.h"
#include <functional>

CYZPP_BEGIN

class EventLoop;

class Channel {
 public:
  typedef int PollEvents;
  typedef int SocketFD;
  typedef std::function<void()> EventCallback;

  enum class Status {
    NO_POLLER,
    IN_POLLER,
    POLLING
  };

  Channel(SocketFD fd, EventLoop* loop);

  Channel(const Channel&) = delete;

  Channel &operator=(const Channel&) = delete;

  // setter
  void setReadCallback(const EventCallback & call) { read_call_ = call; }

  void setWriteCallback(const EventCallback & call) { write_call_ = call; }

  void setErrorCallback(const EventCallback & call) { error_call_ = call; }

  void setREvents(PollEvents events) { events_ = events; }

  void setStatus(Status status) { status_ = status; }

  // getter

  Status getStatus() const { return status_; }

  PollEvents getEvents() const { return events_; }

  SocketFD getSockfd() const { return sockfd_; }

  // 
  void enableRead(bool on);

  void enableWrite(bool on);

  void handleEvents();

 private:
  void updateState();

  // interest event
  PollEvents events_;

  // wait to handle by handleEvents()
  PollEvents revents_;

  // managed sockfd
  SocketFD sockfd_;
  
  // owner event loop
  EventLoop* event_loop_;
  Status status_;

  // event callback fucntion
  EventCallback read_call_;
  EventCallback write_call_;
  EventCallback error_call_;
};

CYZPP_END

#endif  // !_CYZPP_CHANNEL_H__