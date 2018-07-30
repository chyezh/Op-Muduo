#ifndef _CYZPP_CIRCULAR_BUFFER_H__
#define _CYZPP_CIRCULAR_BUFFER_H__

#include "../common.h"
#include <vector>

CYZPP_BEGIN

class CircularBuffer{
 public:
  CircularBuffer();

  ssize_t readFrom(int sockfd);

  size_t read(char *buffer, size_t len);

  ssize_t writeTo(int sockfd);

  size_t write(const char* buffer, size_t len);

  size_t readableNumber() const { return data_.size() - rest_size_; }

 private:
  std::vector<char> data_;
  size_t begin_;
  size_t end_;
  size_t rest_size_;
};

CYZPP_END

#endif // !_CYZPP_CIRCULAR_BUFFER_H__
