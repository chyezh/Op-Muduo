#include "tcp_server.h"
#include "event_loop.h"
#include "tcp_connection.h"
#include <cassert>

CYZPP_BEGIN

TcpServer::TcpServer(EventLoop *loop, const InternetAddress &address)
    : owner_event_loop_(loop), acceptor_(loop, address), is_start_(false) {}

void TcpServer::start() {
  acceptor_.setArrivedConnectonCallback([this](Socket &s, InternetAddress &a){
    newConnectionArrived(s, a);
  });
  acceptor_.listen();
}

TcpServer::~TcpServer() {

}

void TcpServer::setEstablishedCallback(const EstablishedCallback &call) {
  established_callback_ = call;
}

void TcpServer::setMessageCallback(const MessageCallback &call) {
  message_callback_ = call;
}

void TcpServer::newConnectionArrived(Socket &sock, InternetAddress &address) {
  owner_event_loop_->assertIsLoopingThread();
  // create new TcpConnection and add to map
  TcpConnectionPtr new_connection = std::make_shared<TcpConnection>(owner_event_loop_, sock, address);
  connection_set_.emplace(new_connection);
  // set new handler
  new_connection->setEstablishedCallback(established_callback_);
  new_connection->setMessageCallback(message_callback_);
  new_connection->setCloseCallback([this](TcpConnectionPtr conn){ removeConnection(conn); });
  // start work
  new_connection->connetionEstablish();
}

void TcpServer::removeConnection(TcpConnectionPtr connection) {
  owner_event_loop_->assertIsLoopingThread();
  size_t remove_num = connection_set_.erase(connection);
  // check removed
  assert(remove_num == 1);
}

CYZPP_END