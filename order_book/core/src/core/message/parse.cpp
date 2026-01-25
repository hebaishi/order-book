#include "token_parser.hpp"
#include <exception>
#include <iostream>
#include <optional>
#include <order_book/core/message.hpp>
#include <order_book/core/message/new_order.hpp>
#include <order_book/core/message/parse.hpp>
#include <string_view>
namespace order_book::core::message {


std::optional<order_book::core::Message> Parse(std::string_view buffer)
{
  if (buffer.empty()) { return {}; }
  if (buffer[0] == 'N' && buffer.size() >= 2) {
    try {
      const auto payload = buffer.substr(2);
      TokenParser parser{ payload };
      NewOrder new_order;
      parser.Process(new_order.user_id);
      parser.Process(new_order.symbol);
      parser.Process(new_order.price);
      parser.Process(new_order.quantity);
      parser.Process(new_order.side);
      return new_order;
    } catch (const std::exception &ex) {
      std::cout << "Failed to parse: " << ex.what() << "\n";
      return {};
    }
  }
  return {};
}
}// namespace order_book::core::message
