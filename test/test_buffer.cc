#include "../Op-Muduo/network/circular_buffer.h"
#include "../Op-Muduo/network/socket.h"


int main() {
  cyzpp::Socket sock{AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP};
  cyzpp::InternetAddress addr{AF_INET, "192.168.1.128", 43999};
  sock.bind(addr);
  sock.listen();
  cyzpp::Socket sock1;
  while(sock1.nativeHandler() == -1)
    sock1 = sock.accept(addr);
  cyzpp::CircularBuffer buffer1;
  cyzpp::CircularBuffer buffer2;
  char buff[1024];
  while(1) {
    buffer1.readFrom(sock1.nativeHandler());
    size_t t = 0;
    while((t = buffer1.read(buff,1024)) != 0) {
      buffer2.write(buff, t);
    }
    buffer2.writeTo(sock1.nativeHandler());
  }
}