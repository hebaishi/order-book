#pragma once
#include <order_book/core/order.hpp>
namespace order_book::core {
struct Trade
{
  order::Id buy_id;
  order::Id sell_id;
  int quantity;
  int price;
  bool operator==(const Trade &other) const = default;
};
}// namespace order_book::core
