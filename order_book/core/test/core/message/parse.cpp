#include <catch2/catch_test_macros.hpp>
#include <order_book/core/message/parse.hpp>
#include <string_view>
#include <variant>

#include <order_book/core/message/new_order.hpp>
#include <order_book/core/order/side.hpp>
#include <order_book/core/order/symbol.hpp>
namespace order_book::core::message {
template<class... Ts> struct overloaded : Ts...
{
  using Ts::operator()...;
};


TEST_CASE("Successfully parses a well-formed NewOrder message",
  "[new_order_parse]")// NOLINT(readability-function-cognitive-complexity)
{
  // `N, user(int), symbol(string), price(int), qty(int), side(char B or S)`
  const auto message_string = std::string_view("N,235,GOLD,3999,5333,B");
  const auto message = Parse(message_string);
  CHECK(message.has_value());
  if (message.has_value()) {// NOLINT(readability-function-cognitive-complexity)
    std::visit(overloaded{ [&](const NewOrder &new_order) {// NOLINT(readability-function-cognitive-complexity)
      CHECK(new_order.user_id == 235);
      CHECK(new_order.symbol == order::Symbol{ "GOLD" });
      CHECK(new_order.price == 3999);
      CHECK(new_order.quantity == 5333);
      CHECK(new_order.side == order::Side::Buy);
    } },
      message.value());
  }
}
TEST_CASE("Fails to parse a malformed NewOrder message",
  "[new_order_parse]")// NOLINT(readability-function-cognitive-complexity)
{
  const auto message_string = std::string_view("Beans on toast with cheese");
  const auto message = Parse(message_string);
  CHECK_FALSE(message.has_value());
}
}// namespace order_book::core::message
