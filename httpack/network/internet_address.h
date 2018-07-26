#ifndef _CYZPP_INTERNET_ADDRESS_H__
#define _CYZPP_INTERNET_ADDRESS_H__

#include <netinet/in.h>
#include <string>
#include "../common.h"

CYZPP_BEGIN

class InternetAddress {
 public:
  typedef sa_family_t family_t;
  typedef in_port_t port_t;
  typedef in_addr_t address_t;
  typedef in6_addr address6_t;
  typedef struct sockaddr_storage storage_t;

  enum class trans_opt { peer, local };

  InternetAddress();
  
  InternetAddress(family_t family, const std::string &address, port_t port);

  explicit InternetAddress(const struct sockaddr_in &address);

  explicit InternetAddress(const struct sockaddr_in6 &address);

  explicit InternetAddress(const int sockfd, trans_opt option);

  const struct sockaddr *getNativeAddress() const {
    return reinterpret_cast<const struct sockaddr *>(&address_);
  }

  sa_family_t family() const { return address_.ss_family; }

  port_t port() const;

  std::string ip() const;

  std::string address() const;  // ip/port

 private:
  storage_t address_;
};

CYZPP_END

#endif  // !_CYZPP_INTERNET_ADDRESS_H__