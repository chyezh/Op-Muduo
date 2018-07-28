#include "../httpack/network/event_loop.h"
#include "../httpack/network/tcp_acceptor.h"

void new_connection(cyzpp::Socket& sockfd, cyzpp::InternetAddress &addr, std::string str) {
  printf("new connection come from: %s to port %s\n", addr.address().c_str(), str.c_str());
  sockfd.close();
}

void thread_func() {
  cyzpp::EventLoop loop;
  cyzpp::TcpAcceptor accptor(&loop, cyzpp::InternetAddress{AF_INET, "192.168.1.128", 43999});
  accptor.setArrivedConnectonCallback([](cyzpp::Socket& s, cyzpp::InternetAddress &a){ new_connection(s,a,"1"); });
  accptor.listen();
  cyzpp::TcpAcceptor accptor2(&loop, cyzpp::InternetAddress{AF_INET, "192.168.1.128", 49999});
  accptor2.setArrivedConnectonCallback([](cyzpp::Socket& s, cyzpp::InternetAddress &a){ new_connection(s,a,"2"); });
  accptor2.listen();
  loop.loop();
}

int main() {
    std::thread t1(thread_func);
    t1.detach();
    getchar();
}
