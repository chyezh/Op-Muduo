#include "internet_address.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstring>

CYZPP_BEGIN

InternetAddress::InternetAddress() {
  bzero(&address_, sizeof(storage_t));
}

InternetAddress::InternetAddress(family_t family, const std::string &address,
                                 port_t port) {
  bzero(&address_, sizeof(storage_t));
  address_.ss_family = family;
  if (family == AF_INET) {
    sockaddr_in *address_ptr = reinterpret_cast<sockaddr_in *>(&address_);
    address_ptr->sin_port = htons(port);
    inet_pton(AF_INET, address.c_str(), &address_ptr->sin_addr);
  } else if(family == AF_INET6) {
    sockaddr_in6 *address_ptr = reinterpret_cast<sockaddr_in6 *>(&address_);
    address_ptr->sin6_port = htons(port);
    inet_pton(AF_INET6, address.c_str(), &address_ptr->sin6_addr);
  }
}

InternetAddress::InternetAddress(const struct sockaddr_in &address) {
  std::memcpy(&address_, &address, sizeof(sockaddr_in));
}

InternetAddress::InternetAddress(const struct sockaddr_in6 &address) {
  std::memcpy(&address_, &address, sizeof(sockaddr_in6));
}

InternetAddress::InternetAddress(const int sockfd, trans_opt option) {
  socklen_t sz = sizeof(struct sockaddr_storage);
  if (option == trans_opt::local) {
    getsockname(sockfd, reinterpret_cast<struct sockaddr *>(&address_), &sz);
  } else if (option == trans_opt::peer) {
    getpeername(sockfd, reinterpret_cast<struct sockaddr *>(&address_), &sz);
  }
}

InternetAddress::port_t InternetAddress::port() const {
  port_t port_number = 0;
  switch (address_.ss_family) {
    case AF_INET:
      port_number = reinterpret_cast<const sockaddr_in *>(&address_)->sin_port;
      break;
    case AF_INET6:
      port_number =
          reinterpret_cast<const sockaddr_in6 *>(&address_)->sin6_port;
      break;
  }
  return ntohs(port_number);
}

std::string InternetAddress::ip() const {
  char s[INET6_ADDRSTRLEN];
  switch (address_.ss_family) {
    case AF_INET:
      inet_ntop(AF_INET,
                &reinterpret_cast<const sockaddr_in *>(&address_)->sin_addr, s,
                INET_ADDRSTRLEN);
      break;
    case AF_INET6:
      inet_ntop(AF_INET6,
                &reinterpret_cast<const sockaddr_in6 *>(&address_)->sin6_addr,
                s, INET6_ADDRSTRLEN);
      break;
  }
  return std::string(s);
}

std::string InternetAddress::address() const {
  std::string ip_string = ip();
  port_t port_num = port();
  return ip_string + '/' + std::to_string(port());
}

CYZPP_END
