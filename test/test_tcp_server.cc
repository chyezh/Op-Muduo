#include <string.h>
#include "../Op-Muduo/network/event_loop.h"
#include "../Op-Muduo/network/tcp_connection.h"
#include "../Op-Muduo/network/tcp_server.h"
#include <mutex>

void onConnecion(cyzpp::TcpConnectionPtr ptr) {
  static std::mutex m;
  std::lock_guard<std::mutex> lk(m);
  std::cout << "on Connection with " << ptr->getPeerAddress().address()
            << "work in thread " << std::this_thread::get_id() << std::endl;
}

void onMessage(cyzpp::TcpConnectionPtr ptr) {
  char data[128];
  size_t read_num = 0;
  while ((read_num = ptr->recv(data, 128)) != 0) ptr->send(data, read_num);
}

int main() {
  cyzpp::EventLoop loop;
  cyzpp::InternetAddress addr(AF_INET, "192.168.1.128", 43999);
  cyzpp::TcpServer server(&loop, addr, std::thread::hardware_concurrency() - 1);
  server.setEstablishedCallback(onConnecion);
  server.setMessageCallback(onMessage);
  server.start();
  loop.loop();
}
