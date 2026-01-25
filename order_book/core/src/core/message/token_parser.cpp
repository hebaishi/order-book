#include <stdexcept>
#include <system_error>
#include <cstddef>
#include <string_view>
#include "token_parser.hpp"
#include <charconv>
#include <order_book/core/order/side.hpp>
#include <order_book/core/order/symbol.hpp>

namespace order_book::core::message {
TokenParser::TokenParser(std::string_view buf) : buf_(buf) {}

size_t TokenParser::GetNextComma()
{
  for (auto index = start_; index < buf_.size(); index++) {
    if (buf_[index] == ',') { return index; }
  }
  return buf_.size();
}
void TokenParser::ProcessValue(std::string_view string, int &value)
{
  auto result = std::from_chars(string.begin(), string.end(), value);
  if (result.ec == std::errc::invalid_argument) { throw std::runtime_error("Failed to convert string to int"); }
}
void TokenParser::ProcessValue(std::string_view string, order::Side &value)
{
  if (string == "B") {
    value = order::Side::Buy;
  } else if (string == "S") {
    value = order::Side::Sell;
  } else {
    throw std::runtime_error("Failed to convert enum value");
  }
}
void TokenParser::ProcessValue(std::string_view string, order::Symbol &value)
{
  if (string.size() == order::Symbol::Size) {
    value = order::Symbol{ string };
  } else {
    throw std::runtime_error("Failed to serialize Symbol");
  }
}
}// namespace order_book::core::message
