#include <boost/json/serialize.hpp>
#include <catch2/catch_test_macros.hpp>
#include <optional>
#include <order_book/core/order.hpp>
#include <order_book/core/order/id.hpp>
#include <order_book/core/order/side.hpp>
#include <order_book/core/orders.hpp>
#include <ostream>
#include <sstream>
#include <string>

namespace order_book::core {

constexpr int LowPrice = 100;
constexpr int HighPrice = 200;

// TODO: Move this into order.cpp
std::ostream &operator<<(std::ostream &stream, const Order &order)
{
  namespace js = boost::json;
  std::stringstream symbol_name;
  symbol_name << order.symbol;
  stream << js::serialize(js::object{
    { "quantity", order.quantity },
    { "price", order.price },
    { "user_id", order.user_id },
    { "id", order.id.has_value() ? std::to_string(order.id.value()) : "null" },
    { "symbol", symbol_name.str() },
    { "side", order.side == order::Side::Buy ? "buy" : "sell" },
  });
  return stream;
}

static Order MakeOrder(int price, std::optional<order::Id> order_id = {})
{
  constexpr int FixedUserId = 1;
  constexpr int FixedQuantity = 10;
  return Order{ .quantity = FixedQuantity,
    .price = price,
    .user_id = FixedUserId,
    .id = order_id,
    .symbol = Symbol{ "GOLD" },
    .side = order::Side::Buy };
}

TEST_CASE("Given an empty buy orders list", "[orders]")
{
  BuyOrders buy_orders;
  SECTION("With one order")
  {
    auto first_order_id = buy_orders.Add(MakeOrder(LowPrice));
    REQUIRE(first_order_id == 0);
    SECTION("It returns the only order as the best order")
    {
      const auto best = buy_orders.GetBest();
      REQUIRE(best.has_value());
      REQUIRE(best.value_or(Order{}) == MakeOrder(LowPrice, first_order_id));
    }
    SECTION("It returns a higher price order as the best order")
    {
      auto second_order_id = buy_orders.Add(MakeOrder(HighPrice));
      const auto best = buy_orders.GetBest();
      REQUIRE(second_order_id == 1);
      REQUIRE(best.has_value());
      REQUIRE(best == MakeOrder(HighPrice, second_order_id));
    }
    SECTION("It returns nothing when flushed")
    {
      buy_orders.Flush();
      const auto best = buy_orders.GetBest();
      REQUIRE(!best.has_value());
    }
  }
  SECTION("With a high value then a low value order")
  {
    const auto high_order_id = buy_orders.Add(MakeOrder(HighPrice));
    const auto low_order_id = buy_orders.Add(MakeOrder(LowPrice));
    static_cast<void>(low_order_id);
    SECTION("It returns the highest price order")
    {
      const auto best_order = buy_orders.GetBest();
      REQUIRE(best_order.has_value());
      REQUIRE(best_order.value_or(Order{}) == MakeOrder(HighPrice, high_order_id));
    }
  }
  SECTION("With a low value then a high value order")
  {
    const auto low_order_id = buy_orders.Add(MakeOrder(LowPrice));
    const auto high_order_id = buy_orders.Add(MakeOrder(HighPrice));
    static_cast<void>(low_order_id);
    SECTION("It returns the highest price order")
    {
      const auto best_order = buy_orders.GetBest();
      REQUIRE(best_order.has_value());
      REQUIRE(best_order.value_or(Order{}) == MakeOrder(HighPrice, high_order_id));
    }
  }
}

TEST_CASE("Given an empty sell orders list", "[orders]")
{
  SellOrders sell_orders;
  SECTION("With one order")
  {
    const auto first_order_id = sell_orders.Add(MakeOrder(HighPrice));
    REQUIRE(first_order_id == 0);
    SECTION("It returns the only order as the best order")
    {
      const auto best = sell_orders.GetBest();
      REQUIRE(best.has_value());
      REQUIRE(best.value_or(Order{}) == MakeOrder(HighPrice, first_order_id));
    }
    SECTION("It returns a lower price order as the best order")
    {
      const auto second_order_id = sell_orders.Add(MakeOrder(LowPrice));
      const auto best = sell_orders.GetBest();
      REQUIRE(second_order_id == 1);
      REQUIRE(best.has_value());
      REQUIRE(best == MakeOrder(LowPrice, second_order_id));
    }
  }
  SECTION("With a high value then a low value order")
  {
    const auto high_order_id = sell_orders.Add(MakeOrder(HighPrice));
    const auto low_order_id = sell_orders.Add(MakeOrder(LowPrice));
    static_cast<void>(high_order_id);
    static_cast<void>(low_order_id);
    SECTION("It returns the lowest price order")
    {
      const auto best_order = sell_orders.GetBest();
      REQUIRE(best_order.has_value());
      REQUIRE(best_order.value_or(Order{}) == MakeOrder(LowPrice, low_order_id));
    }
  }
  SECTION("With a low value then a high value order")
  {
    const auto low_order_id = sell_orders.Add(MakeOrder(LowPrice));
    const auto high_order_id = sell_orders.Add(MakeOrder(HighPrice));
    static_cast<void>(high_order_id);
    SECTION("It returns the lowest price order")
    {
      const auto best_order = sell_orders.GetBest();
      REQUIRE(best_order.has_value());
      REQUIRE(best_order.value_or(Order{}) == MakeOrder(LowPrice, low_order_id));
    }
  }
}
}// namespace order_book::core
