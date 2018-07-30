#include "tcp_server.h"
#include <cassert>
#include "event_loop.h"
#include "tcp_connection.h"

CYZPP_BEGIN

TcpServer::TcpServer(EventLoop *loop, const InternetAddress &address,
                     size_t thread_num)
    : owner_event_loop_(loop),
      acceptor_(loop, address),
      event_loop_pool_(loop, thread_num),
      is_start_(false) {}

void TcpServer::start() {
  event_loop_pool_.start();
  acceptor_.setArrivedConnectonCallback(
      [this](Socket &s, InternetAddress &a) { newConnectionArrived(s, a); });
  acceptor_.listen();
}

TcpServer::~TcpServer() {}

void TcpServer::setEstablishedCallback(const EstablishedCallback &call) {
  established_callback_ = call;
}

void TcpServer::setMessageCallback(const MessageCallback &call) {
  message_callback_ = call;
}

void TcpServer::newConnectionArrived(Socket &sock, InternetAddress &address) {
  owner_event_loop_->assertIsLoopingThread();
  // create new TcpConnection and add to map
  EventLoop *next_loop = event_loop_pool_.getLoop();
  TcpConnectionPtr new_connection =
      std::make_shared<TcpConnection>(next_loop, sock, address);
  connection_set_.emplace(new_connection);
  // set new handler
  new_connection->setEstablishedCallback(established_callback_);
  new_connection->setMessageCallback(message_callback_);
  // remvoe Connection will call in the io thread
  new_connection->setCloseCallback(
      [this](TcpConnectionPtr conn) { removeConnection(conn); });
  // start work in the io loop
  next_loop->runInLoop(
      [new_connection]() { new_connection->connetionEstablish(); });
}

void TcpServer::removeConnection(TcpConnectionPtr connection) {
  // call in io thread
  // forward the task to the TcpServer owner_event_loop_ thread
  owner_event_loop_->runInLoop(
      [this, connection]() { removeConnectionInLoop(connection); });
}

void TcpServer::removeConnectionInLoop(TcpConnectionPtr connection) {
  owner_event_loop_->assertIsLoopingThread();
  size_t remove_num = connection_set_.erase(connection);
  // check removed
  assert(remove_num == 1);
  // must run in io loop, delay the destruction of connection until the io
  // channel callback of connection finish.
  connection->getOwnerLoop()->queueInLoop(
      [connection]() { connection->destroyConnection(); });
}

CYZPP_END