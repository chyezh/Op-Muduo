#include <cstring>
#include <mutex>
#include "../Op-Muduo/network/event_loop.h"
#include "../Op-Muduo/network/tcp_connection.h"
#include "../Op-Muduo/network/tcp_server.h"

void onConnecion(cyzpp::TcpConnectionPtr ptr) {
  static std::mutex m;
  std::lock_guard<std::mutex> lk(m);
  std::cout << "on Connection with " << ptr->getPeerAddress().address()
            << ";work in thread " << std::this_thread::get_id() << std::endl;
}

class OnMessage {
 public:
  void operator()(cyzpp::TcpConnectionPtr ptr) {
    if (buffer_.size() < ptr->getReadableNumber())
      buffer_.resize(ptr->getReadableNumber());
    size_t read_num = ptr->recv(buffer_.data(), buffer_.size());
    ptr->send(buffer_.data(), read_num);
  }

 private:
  std::vector<char> buffer_;
};

int main() {
  cyzpp::EventLoop loop;
  cyzpp::InternetAddress addr(AF_INET, "192.168.1.128", 43999);
  cyzpp::TcpServer server(&loop, addr, std::thread::hardware_concurrency() - 1);
  server.setEstablishedCallback(onConnecion);
  server.setMessageCallback(OnMessage{});
  server.start();
  loop.loop();
}
