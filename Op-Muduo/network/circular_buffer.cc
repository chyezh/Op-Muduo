#include "circular_buffer.h"
#include <errno.h>
#include <sys/uio.h>
#include <unistd.h>
#include <algorithm>

CYZPP_BEGIN

CircularBuffer::CircularBuffer()
    : data_(20), begin_(0), end_(0), rest_size_(data_.size()) {}

ssize_t CircularBuffer::readFrom(int sockfd) {
  char temp_buffer[65536];
  char *data = data_.data();
  size_t buffer_size1 = 0;
  size_t buffer_size2 = 0;
  size_t total_size = data_.size();
  if (rest_size_ != 0) {
    if (end_ >= begin_) {
      buffer_size1 = total_size - end_;
      buffer_size2 = begin_;
    } else
      buffer_size1 = begin_ - end_;
  }
  struct iovec vec[3];
  vec[0].iov_base = data + end_;
  vec[0].iov_len = buffer_size1;
  vec[1].iov_base = data;
  vec[1].iov_len = buffer_size2;
  vec[2].iov_base = temp_buffer;
  vec[2].iov_len = 65536;
  ssize_t read_num = ::readv(sockfd, vec, 3);
  if (read_num <= 0) return read_num;
  if (read_num <= rest_size_) {
    end_ += read_num;
    rest_size_ -= read_num;
  } else {
    data_.resize(std::max(total_size - rest_size_ + read_num, 2 * total_size));
    data = data_.data();
    std::rotate(data, data + begin_, data + total_size);
    // copy temp_buffer to the new container
    std::copy_n(temp_buffer, read_num - rest_size_, data + total_size);
    begin_ = 0;
    end_ = begin_ + (total_size - rest_size_) + read_num;
    rest_size_ = data_.size() - end_;
  }
  end_ %= data_.size();
  return read_num;
}

size_t CircularBuffer::read(char *buffer, size_t len) {
  char *data = data_.data();
  size_t total_size = data_.size();
  size_t exist_size = data_.size() - rest_size_;
  if (exist_size == 0) return 0;
  // flush all data
  if (exist_size <= len) {
    // 2 part
    if (end_ <= begin_) {
      std::copy(data + begin_, data + total_size, buffer);
      std::copy(data, data + end_, buffer + (total_size - begin_));
      // 1 part
    } else {
      std::copy(data + begin_, data + end_, buffer);
    }
    begin_ = end_ = 0;
    rest_size_ = total_size;
    return exist_size;
    // did not flush all data
  } else {
    // 2 part
    if (end_ <= begin_) {
      if (total_size - begin_ >= len) {
        std::copy_n(data + begin_, len, buffer);
      } else {
        std::copy(data + begin_, data + total_size, buffer);
        size_t done = total_size - begin_;
        std::copy_n(data, len - done, buffer + done);
      }
      // 1 part
    } else {
      std::copy_n(data + begin_, len, buffer);
    }
    begin_ += len;
    rest_size_ += len;
    begin_ %= data_.size();
    if (end_ == begin_) begin_ = end_ = 0;
    return len;
  }
}

ssize_t CircularBuffer::writeTo(int sockfd) {
  // 2 part
  char *data = data_.data();
  size_t total_size = data_.size();
  ssize_t write_num = 0;
  // 2 part
  if (end_ < begin_ || (rest_size_ == 0 && begin_ != 0)) {
    struct iovec vec[2];
    vec[0].iov_base = data + begin_;
    vec[0].iov_len = total_size - begin_;
    vec[1].iov_base = data;
    vec[1].iov_len = end_;
    write_num = ::writev(sockfd, vec, 2);
    // 1 part
  } else {
    write_num = ::write(sockfd, data + begin_, total_size - rest_size_);
  }
  if (write_num <= 0) return write_num;
  begin_ += write_num;
  rest_size_ += write_num;
  begin_ %= data_.size();
  if (end_ == begin_) begin_ = end_ = 0;
  return write_num;
}

size_t CircularBuffer::write(const char* buffer, size_t len) {
  size_t total_size = data_.size();
  size_t exist_size = total_size - rest_size_;
  if(rest_size_ < len)
    data_.resize(std::max(exist_size + len, 2 * total_size));
  char * data = data_.data();
  if(end_ < begin_ || (rest_size_ == 0 && begin_ != 0))
    std::rotate(data, data + begin_, data + exist_size);
  std::copy_n(buffer, len, data + exist_size);
  begin_ = 0;
  end_ = exist_size + len;
  rest_size_ = data_.size() - end_;
  end_ %= data_.size();
  return len;
}
CYZPP_END
