#ifndef _CYZPP_NETWORK_SOCKET_H__
#define _CYZPP_NETWORK_SOCKET_H__

#include <sys/socket.h>
#include "common.h"
#include "internet_address.h"

CYZPP_BEGIN

class InternetAddress;

// move-only class
class Socket {
 public:
  typedef int sockfd_t;
  typedef int sock_proto_t;
  typedef int sock_type_t;
  typedef int sock_domain_t;

  // constructor
  Socket() : sockfd_(-1) {}

  explicit Socket(sockfd_t fd) : sockfd_(fd) {}

  Socket(sock_domain_t domain, sock_type_t type, sock_proto_t protocol);

  Socket(const Socket &) = delete;

  Socket(Socket &&sock);

  Socket &operator=(const Socket &) = delete;

  Socket &operator=(Socket &&sock);

  ~Socket() {
    if (valid()) close();
  }

  void enableTcpNoDelay(bool on);

  void enableReuseAddr(bool on);

  void enableReusePort(bool on);

  void enableKeepAlive(bool on);

  bool valid() { return sockfd_ != -1; }

  sockfd_t nativeHandler() const { return sockfd_; }

  void bind(const InternetAddress &address);

  void listen();

  Socket accept(InternetAddress &address);

  bool connect(const InternetAddress &address);

  void close();


 private:
  sockfd_t sockfd_;
};

CYZPP_END

#endif  // _CYZPP_NETWORK_SOCKET_H__