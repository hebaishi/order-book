#pragma once
#include <optional>
#include <order_book/core/order/id.hpp>
#include <order_book/core/order/side.hpp>
#include <order_book/core/order/symbol.hpp>
#include <order_book/core/ticker.hpp>

namespace order_book::core {

struct Order
{
  int quantity{};
  int price{};
  int user_id{};
  std::optional<order::Id> id;
  order::Symbol symbol{};
  order::Side side{ order::Side::Buy };
  bool operator==(const Order &other) const = default;
};

}// namespace order_book::core
