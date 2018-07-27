#include "tcp_acceptor.h"
#include "event_loop.h"

CYZPP_BEGIN

TcpAcceptor::TcpAcceptor(EventLoop *loop, const InternetAddress &address)
    : owner_event_loop_(loop),
      socket_(::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC,
                       IPPROTO_TCP)),
      acceptor_channel_(socket_.nativeHandler(), loop),
      is_listenning_(false) {
  if (socket_.nativeHandler() < 0) {
    ERRLOG << "TcpAcceptor construct failed\n";
    std::terminate();
  }
  // bind address
  socket_.bind(address);
  // socket setup
  socket_.enableReuseAddr(true);
  socket_.enableKeepAlive(true);
  socket_.enableTcpNoDelay(true);
  socket_.enableReusePort(true);
}

void TcpAcceptor::listen() {
  owner_event_loop_->assertIsLoopingThread();
  socket_.listen();
  // bind to the channel and enable to poll the read event
  acceptor_channel_.setReadCallback([this]() { accpetCallback(); });
  acceptor_channel_.enableRead(true);
  is_listenning_ = true;
}

void TcpAcceptor::setConnectonCallback(const ConnectionCallback &call) {
  connection_callback_ = call;
}

void TcpAcceptor::accpetCallback() {
  owner_event_loop_->assertIsLoopingThread();
  InternetAddress accpet_address;
  Socket new_socket = socket_.accept(accpet_address);
  connection_callback_(new_socket, accpet_address);  
}

CYZPP_END
