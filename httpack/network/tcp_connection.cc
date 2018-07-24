#include "tcp_connection.h"

CYZPP_BEGIN

TcpConnection::TcpConnection(TcpConnection &&connect)
    : sock_(std::move(connect.sock_)),
      peer_(sock_.nativeHandler(), InternetAddress::trans_opt::peer),
      local_(sock_.nativeHandler(), InternetAddress::trans_opt::local) {
  
}

TcpConnection& TcpConnection::operator=(TcpConnection&& connect) {
  if(this != &connect) {
      sock_ = std::move(connect.sock_);
      peer_ = peer_;
      local_ = local_;
  }
  return *this;
}



CYZPP_END
