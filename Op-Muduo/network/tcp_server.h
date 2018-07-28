#ifndef _CYZPP_TCP_SERVER_H__
#define _CYZPP_TCP_SERVER_H__

#include <set>
#include <memory>
#include "tcp_acceptor.h"

CYZPP_BEGIN

class TcpConnection;

class TcpServer {
 public:
  typedef
      typename TcpAcceptor::ArrivedConnectionCallback ArrivedConnectionCallback;
  typedef std::function<void(TcpConnectionPtr)> EstablishedCallback;
  typedef std::function<void(TcpConnectionPtr)> MessageCallback;
  typedef std::set<TcpConnectionPtr> TcpConnectionSet;

  TcpServer(EventLoop *loop, const InternetAddress &address);

  TcpServer(const TcpServer &) = delete;

  TcpServer &operator=(const TcpServer &) = delete;

  ~TcpServer();

  void start();

  void setEstablishedCallback(const EstablishedCallback &call);

  void setMessageCallback(const MessageCallback &call);

 private:
  // for acceptor call back
  void newConnectionArrived(Socket &sock, InternetAddress &address);

  void removeConnection(TcpConnectionPtr connection);

  EventLoop *owner_event_loop_;

  TcpAcceptor acceptor_;
  TcpConnectionSet connection_set_;

  // callback for coming connection
  // on connection
  EstablishedCallback established_callback_;
  // on message comming
  MessageCallback message_callback_;

  // is started?
  bool is_start_;
};

CYZPP_END

#endif  // !_CYZPP_TCP_SERVER_H__