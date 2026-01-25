#pragma once
#include <order_book/core/order.hpp>
namespace order_book::core::message {
struct NewOrder
{
  int quantity{};
  int price{};
  int user_id{};
  order::Symbol symbol{};
  order::Side side{ order::Side::Buy };
  bool operator==(const NewOrder &other) const = default;
  template<class Archive> void serialize(Archive &archive, const unsigned int /*version*/)
  {
    archive & quantity;
    archive & price;
    archive & user_id;
    archive & symbol;
    archive & side;
  }
};
}// namespace order_book::core::message
