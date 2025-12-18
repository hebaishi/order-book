#include <catch2/catch_test_macros.hpp>
#include <order_book/core/message/new_order.hpp>
#include <order_book/core/order/side.hpp>
#include <order_book/core/orders.hpp>
#include <order_book/core/state.hpp>
#include <order_book/core/state/process.hpp>


namespace order_book::core::state {
constexpr int DefaultOrderQuantity = 100;
constexpr int DefaultOrderPrice = 100;
auto DefaultSymbol() { return Symbol{ "GOLD" }; }
TEST_CASE("Given an empty state", "[Process]")
{
  order_book::core::State state;
  CHECK(state.books.empty());
  SECTION("Processing a new buy order creates the book for it, adding to the buy orders",
    "[Req-processing-new-book-creation][Req-processing-new-book-updated]")
  {
    Process(state,
      order_book::core::message::NewOrder{
        .quantity = DefaultOrderQuantity,
        .price = DefaultOrderPrice,
        .user_id = 1,
        .id = {},
        .symbol = DefaultSymbol(),
        .side = order::Side::Buy,
      });
    CHECK(state.books.size() == 1);
    CHECK(state.books[0].buy_orders.orders.size() == 1);
  }
  SECTION("Processing a new sell order creates the book for it, adding to the sell orders",
    "[Req-processing-new-book-creation][Req-processing-new-book-updated]")
  {
    Process(state,
      order_book::core::message::NewOrder{
        .quantity = DefaultOrderQuantity,
        .price = DefaultOrderPrice,
        .user_id = 1,
        .id = {},
        .symbol = DefaultSymbol(),
        .side = order::Side::Sell,
      });
    CHECK(state.books.size() == 1);
    CHECK(state.books[0].sell_orders.orders.size() == 1);
  }
}
}// namespace order_book::core::state
