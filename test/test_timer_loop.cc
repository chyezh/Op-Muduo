#include "../httpack/network/event_loop.h"

cyzpp::EventLoop* gloop = nullptr;

void thread_func() {
    cyzpp::EventLoop loop;
    gloop = &loop;
    loop.loop();
}

void timer_func(std::string str) {
  std::cout << str << '\n';
}

int main() {
  using namespace std::chrono_literals;
  std::thread t1(thread_func);
  t1.detach();
  while(gloop == nullptr);
  gloop->runEvery([]() { timer_func("0"); }, 3s);
  gloop->runAfter([]() { timer_func("1"); }, 5s);
  gloop->runAfter([]() { timer_func("2"); }, 4s);
  gloop->runAfter([]() { timer_func("3"); }, 3s);
  gloop->runAfter([]() { timer_func("4"); }, 2s);
  gloop->runAfter([]() { timer_func("5"); }, 1s);
  getchar();
  gloop->quit();
  while(gloop->isRunning());
  return 0;
}
