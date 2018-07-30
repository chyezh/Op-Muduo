#ifndef _CYZPP_TCP_CONNECTION_H__
#define _CYZPP_TCP_CONNECTION_H__

#include "../common.h"
#include "socket.h"
#include "circular_buffer.h"
#include <memory>
#include <functional>
#include <atomic>
#include <list>

CYZPP_BEGIN

class Channel;

class EventLoop;

class CircularBuffer;

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
 public:
  typedef std::function<void(TcpConnectionPtr)> EstablishedCallback;
  typedef std::function<void(TcpConnectionPtr)> MessageCallback;
  typedef std::function<void(TcpConnectionPtr)> ErrorCallback;
  typedef std::function<void(TcpConnectionPtr)> CloseCallback;
  typedef uint64_t TcpConnectionID;

  enum class Status {
    CONNECTING,
    CONNECTED,
    DISCONNECTING,
    DISCONNECTED
  };

  TcpConnection(EventLoop *loop, Socket &socket, const InternetAddress& address);

  TcpConnection(const TcpConnection&) = delete;

  TcpConnection& operator=(const TcpConnection&) = delete;

  uint64_t getID() const { return id_; }

  void connetionEstablish();

  void setEstablishedCallback(const EstablishedCallback &call);

  void setMessageCallback(const MessageCallback &call);

  void setCloseCallback(const CloseCallback &call);

  InternetAddress getPeerAddress() { return peer_; }

  size_t getReadableNumber() { return read_buffer_.readableNumber(); }

  EventLoop *getOwnerLoop() { return owner_event_loop_; }

  size_t recv(char *buffer, size_t len);

  size_t recvNumber();

  void send(const char* buffer, size_t len);

  // for connection read callback
  void handleRead();

  // for channel write callback
  void handleWrite();

  // for channel error callback
  void handleError();

  // for close the connection
  void handleClose();

  // destroy connection
  void destroyConnection();

  // generate unique id for timer
  static TcpConnectionID generateID();

 private:
  void sendInLoop(const char* buffer, size_t len);

  void sendInLoop(const std::string& str);

  // owner loop
  EventLoop* owner_event_loop_;
  
  // hold socket
  Socket socket_;
  
  // envent distribution channel
  std::unique_ptr<Channel> channel_;

  // buffers
  // read buffer
  CircularBuffer read_buffer_;
  // write buffer
  CircularBuffer write_buffer_;

  // call back
  MessageCallback message_callback_;

  EstablishedCallback established_callback_;
  
  CloseCallback close_callback_;
  
  // peer address
  InternetAddress peer_;

  // unique id for every connection
  uint64_t id_;

  // state of this connection
  Status state_;

  // unique id generator
  static std::atomic_uint64_t id_generator;
};

namespace detail {
  // initialize the connecton working environment
  class EnvironmentInitializer {
    public:
     EnvironmentInitializer();

     EnvironmentInitializer(const EnvironmentInitializer&) = delete;

     EnvironmentInitializer& operator=(const EnvironmentInitializer&);
  };
};

CYZPP_END

#endif // !_CYZPP_TCP_CONNECTION_H__