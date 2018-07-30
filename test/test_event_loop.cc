#include "../Op-Muduo/network/event_loop.h"

void thread_func() {
  cyzpp::EventLoop loop;
  loop.loop();
}

int main() {
  cyzpp::EventLoop loop;
  std::thread t1(thread_func);
  t1.detach();
  loop.loop();
}
