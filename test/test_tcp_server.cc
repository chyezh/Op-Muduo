#include "../httpack/network/tcp_server.h"
#include "../httpack/network/event_loop.h"
#include "../httpack/network/tcp_connection.h"
#include <string.h>

void onConnecion(cyzpp::TcpConnectionPtr ptr) {
    printf("on Connection: \n");
}

std::string s("fuck you\n");

void onMessage(cyzpp::TcpConnectionPtr ptr) {
  char data[128];
  auto read_num = ptr->recv(data, 128);
  ptr->send(s.data(), 10);
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
