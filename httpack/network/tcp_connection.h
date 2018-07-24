#ifndef _CYZPP_TCP_CONNECTION_H__
#define _CYZPP_TCP_CONNECTION_H__

#include "socket.h"

CYZPP_BEGIN

class TcpConnection {
 public:
  TcpConnection();

  TcpConnection(const TcpConnection&) = delete;

  TcpConnection& operator=(const TcpConnection&) = delete;

  TcpConnection(TcpConnection &&connect);

  TcpConnection& operator=(TcpConnection&& connect);


 private:
  Socket sock_;
  InternetAddress peer_;
  InternetAddress local_;
};

CYZPP_END

#endif // !_CYZPP_TCP_CONNECTION_H__