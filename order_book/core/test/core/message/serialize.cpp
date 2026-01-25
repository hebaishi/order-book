#include <catch2/catch_test_macros.hpp>
#include <order_book/core/message/serialize.hpp>
#include <string>

#include <order_book/core/message/new_order.hpp>
#include <order_book/core/order/side.hpp>
#include <order_book/core/order/symbol.hpp>
namespace order_book::core::message {


TEST_CASE("Successfully serializer NewOrder message",
  "[new_order_serialize]")// NOLINT(readability-function-cognitive-complexity)
{
  // `N, user(int), symbol(string), price(int), qty(int), side(char B or S)`
  const auto new_order = NewOrder{
    .quantity = 5333, .price = 3999, .user_id = 235, .symbol = order::Symbol{ "GOLD" }, .side = order::Side::Buy
  };
  const auto message = Serialize(new_order);
  const auto expected = std::string("N,235,GOLD,3999,5333,B");
  CHECK(message.size() == expected.size());
  CHECK(message == expected);
}
}// namespace order_book::core::message
