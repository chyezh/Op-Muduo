#include "tcp_connection.h"
#include <unistd.h>
#include <cassert>
#include <algorithm>
#include "channel.h"
#include "event_loop.h"

CYZPP_BEGIN

std::atomic_uint64_t TcpConnection::id_generator;

TcpConnection::TcpConnectionID TcpConnection::generateID() {
  return id_generator.fetch_add(1);
}

TcpConnection::TcpConnection(EventLoop *loop, Socket &socket,
                             const InternetAddress &address)
    : owner_event_loop_(loop),
      socket_(std::move(socket)),
      channel_(std::make_unique<Channel>(socket_.nativeHandler(), loop)),
      peer_(address),
      id_(generateID()),
      state_(Status::CONNECTED) {
  channel_->setReadCallback([this]() { handleRead(); });
  channel_->setWriteCallback([this]() { handleWrite(); });
  channel_->setErrorCallback([this]() { handleRead(); });
}

void TcpConnection::connetionEstablish() {
  owner_event_loop_->assertIsLoopingThread();
  channel_->enableRead(true);
  state_ = Status::ESTABLISHED;
  established_callback_(shared_from_this());
}

void TcpConnection::setEstablishedCallback(const EstablishedCallback &call) {
  established_callback_ = call;
}

void TcpConnection::setMessageCallback(const MessageCallback &call) {
  message_callback_ = call;
}

void TcpConnection::setCloseCallback(const CloseCallback &call) {
  close_callback_ = call;
}

size_t TcpConnection::recv(char *buffer, size_t len) {
  return read_buffer_.read(static_cast<char *>(buffer), len);
}

size_t TcpConnection::recvNumber() { return read_buffer_.readableNumber(); }

void TcpConnection::send(const char *buffer, size_t len) {
  if (state_ == Status::ESTABLISHED) {
    if (owner_event_loop_->isLoopingThread()) {
      // run directly in io loop
      sendInLoop(buffer, len);
    } else {
      // else, copy buffer and send to the io loop
      owner_event_loop_->runInLoop(
          [this, str = std::string(buffer, len)]() { sendInLoop(str); });
    }
  }
}

void TcpConnection::sendInLoop(const char *buffer, size_t len) {
  owner_event_loop_->assertIsLoopingThread();
  ssize_t write_num = 0;
  if(!channel_->isWriting() && write_buffer_.readableNumber() == 0) {
    write_num = ::write(socket_.nativeHandler(), buffer, len);
  }
  if(write_num != len) {
    write_buffer_.write(buffer + write_num, len - write_num);
  }
  if(!channel_->isWriting() && write_buffer_.readableNumber() != 0) {
    channel_->enableWrite(true);
  }
}

void TcpConnection::sendInLoop(const std::string& str) {
  owner_event_loop_->assertIsLoopingThread();
  sendInLoop(str.data(), str.size());
}

void TcpConnection::handleRead() {
  owner_event_loop_->assertIsLoopingThread();
  assert(state_ == Status::ESTABLISHED);
  // read the message into the buffer
  ssize_t read_num = read_buffer_.readFrom(socket_.nativeHandler());
  if (read_num > 0)
    message_callback_(shared_from_this());
  else if (read_num == 0)
    handleClose();
  else if (read_num < 0)
    handleError();
}

void TcpConnection::handleWrite() {
  owner_event_loop_->assertIsLoopingThread();
  write_buffer_.writeTo(socket_.nativeHandler());
  // if no data wait for sending, disable channel write signal
  if(write_buffer_.readableNumber() == 0)
    channel_->enableWrite(false);
}

void TcpConnection::handleError() {
  owner_event_loop_->assertIsLoopingThread();
}

void TcpConnection::handleClose() {
  owner_event_loop_->assertIsLoopingThread();
  assert(state_ == Status::ESTABLISHED);
  channel_->enableRead(false);
  channel_->enableWrite(false);
  channel_->removeSelf();
  state_ = Status::DISCONNECTED;
  socket_.close();
  close_callback_(shared_from_this());
}

CYZPP_END
