#include "waken_channel.h"
#include <sys/eventfd.h>
#include <unistd.h>
#include "channel.h"

CYZPP_BEGIN

WakenChannel::WakenChannel(EventLoop *loop)
    : owner_event_loop_(loop), is_running_pending_event_(false) {
  eventfd_ = eventfd(1, EFD_NONBLOCK | EFD_CLOEXEC);
  if (eventfd_ < 0) {
    ERRLOG << "WakenChannel construct fail\n";
    std::terminate();
  }
  // enable the channel
  channel_ = std::make_unique<Channel>(eventfd_, loop);
  channel_->setReadCallback([this]() { wakeCallback(); });
  // poll the eventfd
  channel_->enableRead(true);
}

WakenChannel::~WakenChannel() { ::close(eventfd_); }

void WakenChannel::addPendingEvent(const PendingFunction &function) {
  std::lock_guard<std::mutex> lk(pending_event_mutex_);
  pending_event_.emplace_back(function);
}

void WakenChannel::wakeup() {
  uint64_t val = 1;
  ssize_t write_num = ::write(eventfd_, &val, sizeof(uint64_t));
  if (write_num != sizeof(uint64_t)) ERRLOG << "WakenChannel::wake error\n";
}

void WakenChannel::runPendingEvent() {
  is_running_pending_event_ = true;
  std::vector<PendingFunction> pending_func;
  {
    std::lock_guard<std::mutex> lk(pending_event_mutex_);
    std::swap(pending_event_, pending_func);
  }
  for (auto &&func : pending_func) {
    func();
  }
  is_running_pending_event_ = false;
}

// for channel_ read call back
void WakenChannel::wakeCallback() {
  uint64_t val = 0;
  ssize_t read_num = ::read(eventfd_, &val, sizeof(uint64_t));
  if (read_num != sizeof(uint64_t))
    ERRLOG << "WakenChannel::wakeCallback error\n";
}

CYZPP_END
