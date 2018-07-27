#ifndef _CYZPP_TCP_ACCEPTOR_H__
#define _CYZPP_TCP_ACCEPTOR_H__

#include "socket.h"
#include "internet_address.h"
#include "channel.h"

CYZPP_BEGIN

class EventLoop;

class TcpAcceptor {
 public:
  typedef std::function<void(Socket&, InternetAddress&)> ConnectionCallback;

  TcpAcceptor(EventLoop* loop, const InternetAddress& address);

  void listen();

  void setConnectonCallback(const ConnectionCallback &call);

 private:
  void accpetCallback();

  EventLoop* owner_event_loop_;
  Socket socket_;
  Channel acceptor_channel_;
  bool is_listenning_;
  ConnectionCallback connection_callback_;
};

CYZPP_END

#endif // !_CYZPP_TCP_ACCEPTOR_H__