#pragma once
#include <order_book/core/order/side.hpp>
#include <order_book/core/order/symbol.hpp>
#include <string_view>

namespace order_book::core::message {
class TokenParser
{
public:
  explicit TokenParser(std::string_view buf);

  size_t GetNextComma();
  static void ProcessValue(std::string_view string, int &value);

  static void ProcessValue(std::string_view string, order::Side &value);

  static void ProcessValue(std::string_view string, order::Symbol &value);

  template<typename T> void Process(T &value)
  {
    auto comma = GetNextComma();
    const auto token = buf_.substr(start_, comma - start_);
    ProcessValue(token, value);
    start_ = comma + 1;
  }

private:
  std::string_view buf_;
  size_t start_{};
};
}// namespace order_book::core::message
