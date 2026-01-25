#pragma once
#include <boost/serialization/optional.hpp>
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
  order::Id id;
  order::Symbol symbol{};
  order::Side side{ order::Side::Buy };
  bool operator==(const Order &other) const = default;
  template<class Archive> void serialize(Archive &archive, const unsigned int /*version*/)
  {
    archive & quantity;
    archive & price;
    archive & user_id;
    archive & id;
    archive & symbol;
    archive & side;
  }
};

}// namespace order_book::core
