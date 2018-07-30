#include "../Op-Muduo/network/event_loop_thread.h"
#include "../Op-Muduo/network/event_loop.h"

void timer_func(std::string str) {
  std::cout << str;
}

int main() {
  using namespace std::chrono_literals;
  auto gloop = cyzpp::EventLoopThread::makeEventLoop();
  gloop->runEvery([]() { timer_func("0\n"); }, 3s);
  gloop->runAfter([]() { timer_func("1\n"); }, 5s);
  gloop->runAfter([]() { timer_func("2\n"); }, 4s);
  gloop->runAfter([]() { timer_func("3\n"); }, 3s);
  gloop->runAfter([]() { timer_func("4\n"); }, 2s);
  gloop->runAfter([]() { timer_func("5\n"); }, 1s);
  auto gloop2 = cyzpp::EventLoopThread::makeEventLoop();
  gloop2->runEvery([]() { timer_func("0\n"); }, 3s);
  gloop2->runAfter([]() { timer_func("1\n"); }, 5s);
  gloop2->runAfter([]() { timer_func("2\n"); }, 4s);
  gloop2->runAfter([]() { timer_func("3\n"); }, 3s);
  gloop2->runAfter([]() { timer_func("4\n"); }, 2s);
  gloop2->runAfter([]() { timer_func("5\n"); }, 1s);
  getchar();
  gloop->quit();
  gloop2->quit();
  while(gloop->isRunning() || gloop2->isRunning());
  return 0;
}