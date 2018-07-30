#include <string.h>
#include "../Op-Muduo/network/event_loop.h"
#include "../Op-Muduo/network/tcp_connection.h"
#include "../Op-Muduo/network/tcp_server.h"

void onConnecion(cyzpp::TcpConnectionPtr ptr) { printf("on Connection: \n"); }

void onMessage(cyzpp::TcpConnectionPtr ptr) {
  char data[128];
  size_t read_num = 0;
  while((read_num = ptr->recv(data, 128)) != 0)
    ptr->send(data, read_num);
}

int main() {
  cyzpp::EventLoop loop;
  cyzpp::InternetAddress addr(AF_INET, "192.168.1.128", 43999);
  cyzpp::TcpServer server(&loop, addr);
  server.setEstablishedCallback(onConnecion);
  server.setMessageCallback(onMessage);
  server.start();
  loop.loop();
}
