#pragma once
#include <order_book/core/orders.hpp>
#include <order_book/core/order/symbol.hpp>
namespace order_book::core::order {

struct Book {
  explicit Book(Symbol symbol);
  BuyOrders buy_orders;
  SellOrders sell_orders;
  Symbol symbol{};
};

}// namespace order_book::core::order
