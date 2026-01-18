#include <catch2/catch_test_macros.hpp>
#include <order_book/core/match.hpp>
#include <order_book/core/message/new_order.hpp>
#include <order_book/core/order/book.hpp>
#include <order_book/core/order/side.hpp>
#include <order_book/core/order/symbol.hpp>
#include <order_book/core/state.hpp>
#include <order_book/core/state/process.hpp>
#include <order_book/core/trade.hpp>
namespace order_book::core {
constexpr auto LowPrice = 100;
constexpr auto HighPrice = 101;
constexpr auto Single = 1;
constexpr auto Double = 2;
constexpr auto symbol =  order::Symbol{ "GOLD" };

TEST_CASE("Processing a single buy order results in no match", "[matching]")
{
  core::State state{ .books = core::order::Books{ order::Book{ symbol } } };
  state::Process(state,
    core::message::NewOrder{
      .quantity = 0,
      .price = 0,
      .user_id = 0,
      .id = {},
      .symbol = symbol,
      .side = order::Side::Buy,
    });
  const auto trades = core::Match(state.books[0]);
  REQUIRE(trades.empty());
}
TEST_CASE("Processing a single sell order results in no match", "[matching]")
{
  core::State state{ .books = core::order::Books{ order::Book{ symbol } } };
  state::Process(state,
    core::message::NewOrder{
      .quantity = 0,
      .price = 0,
      .user_id = 0,
      .id = {},
      .symbol = symbol,
      .side = order::Side::Sell,
    });
  const auto trades = core::Match(state.books[0]);
  REQUIRE(trades.empty());
}
TEST_CASE("Processing a orders where buy price is lower than sell price results in no match", "[matching]")
{
  core::State state{ .books = core::order::Books{ order::Book{ symbol } } };
  state::Process(state,
    core::message::NewOrder{
      .quantity = Single,
      .price = LowPrice,
      .user_id = 0,
      .id = {},
      .symbol = symbol,
      .side = order::Side::Sell,
    });
  state::Process(state,
    core::message::NewOrder{
      .quantity = Single,
      .price = HighPrice,
      .user_id = 0,
      .id = {},
      .symbol = symbol,
      .side = order::Side::Sell,
    });
  const auto trades = Match(state.books[0]);
  REQUIRE(trades.empty());
}
TEST_CASE("Processing orders where buy exceeds sell results in a single match", "[matching]")
{
  const auto quantity = Single;
  core::State state{ .books = core::order::Books{ order::Book{ symbol } } };
  const auto buy_id = state::Process(state,
    core::message::NewOrder{
      .quantity = quantity,
      .price = HighPrice,
      .user_id = 0,
      .id = {},
      .symbol = symbol,
      .side = order::Side::Buy,
    });
  const auto sell_id = state::Process(state,
    core::message::NewOrder{
      .quantity = quantity,
      .price = LowPrice,
      .user_id = 0,
      .id = {},
      .symbol = symbol,
      .side = order::Side::Sell,
    });
  const auto trades = Match(state.books[0]);
  REQUIRE_FALSE(trades.empty());
  REQUIRE(trades.size() == 1);
  REQUIRE(trades[0] == Trade{ .buy_id = buy_id, .sell_id = sell_id, .quantity = quantity, .price = LowPrice });
}
TEST_CASE("Processing one buy order with two sell orders that match", "[matching]")
{
  core::State state{ .books = core::order::Books{ order::Book{ symbol } } };
  const auto buy_id = state::Process(state,
    core::message::NewOrder{
      .quantity = Double,
      .price = HighPrice,
      .user_id = 0,
      .id = {},
      .symbol = symbol,
      .side = order::Side::Buy,
    });
  const auto sell_id_one = state::Process(state,
    core::message::NewOrder{
      .quantity = Single,
      .price = LowPrice,
      .user_id = 0,
      .id = {},
      .symbol = symbol,
      .side = order::Side::Sell,
    });
  const auto sell_id_two = state::Process(state,
    core::message::NewOrder{
      .quantity = Single,
      .price = LowPrice,
      .user_id = 0,
      .id = {},
      .symbol = symbol,
      .side = order::Side::Sell,
    });
  const auto trades = Match(state.books[0]);
  REQUIRE_FALSE(trades.empty());
  REQUIRE(trades.size() == 2);
  REQUIRE(trades[0] == Trade{ .buy_id = buy_id, .sell_id = sell_id_two, .quantity = Single, .price = LowPrice });
  REQUIRE(trades[1] == Trade{ .buy_id = buy_id, .sell_id = sell_id_one, .quantity = Single, .price = LowPrice });
}
TEST_CASE("Processing two buy order with one sell orders that match", "[matching]")
{
  core::State state{ .books = core::order::Books{ order::Book{ symbol } } };
  const auto buy_id_one = state::Process(state,
    core::message::NewOrder{
      .quantity = Single,
      .price = HighPrice,
      .user_id = 0,
      .id = {},
      .symbol = symbol,
      .side = order::Side::Buy,
    });
  const auto buy_id_two = state::Process(state,
    core::message::NewOrder{
      .quantity = Single,
      .price = HighPrice,
      .user_id = 0,
      .id = {},
      .symbol = symbol,
      .side = order::Side::Buy,
    });
  const auto sell_id = state::Process(state,
    core::message::NewOrder{
      .quantity = Double,
      .price = LowPrice,
      .user_id = 0,
      .id = {},
      .symbol = symbol,
      .side = order::Side::Sell,
    });
  const auto trades = Match(state.books[0]);
  REQUIRE_FALSE(trades.empty());
  REQUIRE(trades.size() == 2);
  REQUIRE(trades[0] == Trade{ .buy_id = buy_id_two, .sell_id = sell_id, .quantity = Single, .price = LowPrice });
  REQUIRE(trades[1] == Trade{ .buy_id = buy_id_one, .sell_id = sell_id, .quantity = Single, .price = LowPrice });
}
}// namespace order_book::core
