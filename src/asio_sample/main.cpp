#include <boost/asio.hpp>
#include <chrono>
#include <iostream>

int main() {
  boost::asio::io_context ioc;
  boost::asio::steady_timer timer(ioc);
  constexpr auto wait_time = std::chrono::seconds(5);
  timer.expires_after(wait_time);
  timer.async_wait([](const auto& error) {
      if (!error)
      std::cout << "Inside wait callback\n";
      });
  ioc.run();
  return 0;
}
