#include "../httpack/network/channel.h"
#include "../httpack/network/event_loop.h"
#include "../httpack/network/poller.h"
#include <sys/timerfd.h>
#include <cstring>
#include <unistd.h>


void timeout() {
  printf("Timeout!\n");
  cyzpp::EventLoop::getEventLoopThisThread()->quit();
}

int main() {
  cyzpp::EventLoop loop;
  int timer_fd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
  cyzpp::Channel channel(timer_fd, &loop);
  channel.setReadCallback(timeout);
  channel.enableRead(true);
  struct itimerspec howlong;
  bzero(&howlong, sizeof(struct itimerspec));
  howlong.it_value.tv_sec = 5;
  ::timerfd_settime(timer_fd, 0, &howlong, NULL);
  loop.loop();
  ::close(timer_fd);
}