#include <catch2/catch_test_macros.hpp>
#include <optional>
#include <order_book/core/message/new_order.hpp>
#include <order_book/core/order.hpp>
#include <order_book/core/order/side.hpp>
#include <order_book/core/orders.hpp>

namespace order_book::core {

constexpr int LowPrice = 100;
constexpr int HighPrice = 200;


static message::NewOrder MakeOrder(int price)
{
  constexpr int FixedUserId = 1;
  constexpr int FixedQuantity = 10;
  return message::NewOrder{ .quantity = FixedQuantity,
    .price = price,
    .user_id = FixedUserId,
    .symbol = order::Symbol{ "GOLD" },
    .side = order::Side::Buy };
}

bool operator==(const Order &order, const message::NewOrder &new_order)
{
  return new_order.quantity == order.quantity && new_order.price == order.price && new_order.user_id == order.user_id
         && new_order.symbol == order.symbol && new_order.side == order.side;
}

TEST_CASE("Given an empty buy orders list", "[orders]")
{
  BuyOrders buy_orders;
  SECTION("With one order")
  {
    auto first_order_id = buy_orders.Add(MakeOrder(LowPrice));
    CHECK(first_order_id == 0);
    // Req-Orders-get-only
    SECTION("It returns the only order as the best order")
    {
      const auto best = buy_orders.GetBest();
      CHECK(best.has_value());
      CHECK(best.value_or(Order{}) == MakeOrder(LowPrice));
    }
    // Req-Orders-get-best
    SECTION("It returns a higher price order as the best order")
    {
      auto second_order_id = buy_orders.Add(MakeOrder(HighPrice));
      const auto best = buy_orders.GetBest();
      CHECK(second_order_id == 1);
      CHECK(best.has_value());
      CHECK(best == MakeOrder(HighPrice));
    }
    // Req-Orders-return-nothing-empty
    SECTION("It returns nothing when empty")
    {
      buy_orders.Flush();
      const auto best = buy_orders.GetBest();
      CHECK(!best.has_value());
    }
  }
  // Req-Orders-get-best
  SECTION("With a high value then a low value order")
  {
    const auto high_order_id = buy_orders.Add(MakeOrder(HighPrice));
    static_cast<void>(high_order_id);
    const auto low_order_id = buy_orders.Add(MakeOrder(LowPrice));
    static_cast<void>(low_order_id);
    SECTION("It returns the highest price order")
    {
      const auto best_order = buy_orders.GetBest();
      CHECK(best_order.has_value());
      CHECK(best_order.value_or(Order{}) == MakeOrder(HighPrice));
    }
  }
  // Req-Orders-get-best
  SECTION("With a low value then a high value order")
  {
    const auto low_order_id = buy_orders.Add(MakeOrder(LowPrice));
    const auto high_order_id = buy_orders.Add(MakeOrder(HighPrice));
    static_cast<void>(high_order_id);
    static_cast<void>(low_order_id);
    SECTION("It returns the highest price order")
    {
      const auto best_order = buy_orders.GetBest();
      CHECK(best_order.has_value());
      CHECK(best_order.value_or(Order{}) == MakeOrder(HighPrice));
    }
  }
}

TEST_CASE("Given an empty sell orders list", "[orders]")
{
  SellOrders sell_orders;
  SECTION("With one order")
  {
    const auto first_order_id = sell_orders.Add(MakeOrder(HighPrice));
    CHECK(first_order_id == 0);
    // Req-Orders-get-only
    SECTION("It returns the only order as the best order")
    {
      const auto best = sell_orders.GetBest();
      CHECK(best.has_value());
      CHECK(best.value_or(Order{}) == MakeOrder(HighPrice));
    }
    // Req-Orders-get-best
    SECTION("It returns a lower price order as the best order")
    {
      const auto second_order_id = sell_orders.Add(MakeOrder(LowPrice));
      const auto best = sell_orders.GetBest();
      CHECK(second_order_id == 1);
      CHECK(best.has_value());
      CHECK(best == MakeOrder(LowPrice));
    }
  }
  // Req-Orders-get-best
  SECTION("With a high value then a low value order")
  {
    const auto high_order_id = sell_orders.Add(MakeOrder(HighPrice));
    const auto low_order_id = sell_orders.Add(MakeOrder(LowPrice));
    static_cast<void>(high_order_id);
    static_cast<void>(low_order_id);
    SECTION("It returns the lowest price order")
    {
      const auto best_order = sell_orders.GetBest();
      CHECK(best_order.has_value());
      CHECK(best_order.value_or(Order{}) == MakeOrder(LowPrice));
    }
  }
  // Req-Orders-get-best
  SECTION("With a low value then a high value order")
  {
    const auto low_order_id = sell_orders.Add(MakeOrder(LowPrice));
    const auto high_order_id = sell_orders.Add(MakeOrder(HighPrice));
    static_cast<void>(high_order_id);
    static_cast<void>(low_order_id);
    SECTION("It returns the lowest price order")
    {
      const auto best_order = sell_orders.GetBest();
      CHECK(best_order.has_value());
      CHECK(best_order.value_or(Order{}) == MakeOrder(LowPrice));
    }
  }
}
}// namespace order_book::core
