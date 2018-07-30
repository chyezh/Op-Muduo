# Op-Muduo

一个参考muduo实现的TCP网络库,用于理解多线程并发服务器的实现。

目前仅仅实现了大致的框架与服务器类。

[muduo]https://github.com/chenshuo/muduo

## 使用

### 实现ECHO服务器

```
void onConnecion(cyzpp::TcpConnectionPtr ptr) {
  static std::mutex m;
  std::lock_guard<std::mutex> lk(m);
  std::cout << "on Connection with " << ptr->getPeerAddress().address()
            << ";work in thread " << std::this_thread::get_id() << std::endl;
}

class OnMessage {
 public:
  void operator()(cyzpp::TcpConnectionPtr ptr) {
    if (buffer_.size() < ptr->getReadableNumber())
      buffer_.resize(ptr->getReadableNumber());
    size_t read_num = ptr->recv(buffer_.data(), buffer_.size());
    ptr->send(buffer_.data(), read_num);
  }

 private:
  std::vector<char> buffer_;
};

int main() {
  cyzpp::EventLoop loop;
  cyzpp::InternetAddress addr(AF_INET, "127.0.0.1", 7);
  cyzpp::TcpServer server(&loop, addr, std::thread::hardware_concurrency() - 1);
  server.setEstablishedCallback(onConnecion);
  server.setMessageCallback(OnMessage{});
  server.start();
  loop.loop();
}
```