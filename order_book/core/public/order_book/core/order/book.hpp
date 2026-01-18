#pragma once
#include <core_export.h>
#include <order_book/core/order/symbol.hpp>
#include <order_book/core/orders.hpp>
namespace order_book::core::order {

struct Book
{
  CORE_EXPORT explicit Book(Symbol symbol);
  BuyOrders buy_orders;
  SellOrders sell_orders;
  Symbol symbol{};
};

}// namespace order_book::core::order
