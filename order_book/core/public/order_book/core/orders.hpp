#pragma once
#include <algorithm>
#include <functional>
#include <optional>
#include <order_book/core/order.hpp>
#include <order_book/core/order/side.hpp>

namespace order_book::core {

template<order::Side side> struct Comparator;

template<> struct Comparator<order::Side::Buy>
{
  using Value = decltype([](const Order &first, const Order &second) { return first.price < second.price; });
};

template<> struct Comparator<order::Side::Sell>
{
  using Value = decltype([](const Order &first, const Order &second) { return first.price > second.price; });
};

template<order::Side side> struct Orders
{
  std::vector<Order> orders;
  order::Id current_id{};
  using Element = Order;
  Orders() = default;

  using ComparatorFunction = Comparator<side>::Value;
  [[nodiscard]] order::Id Add(Order order)
  {
    order.id = current_id++;
    orders.push_back(order);
    std::stable_sort(orders.begin(), orders.end(), ComparatorFunction{});
    return order.id.value();
  }

  [[nodiscard]] std::optional<Order> GetBest() const
  {
    if (!orders.empty()) { return orders.back(); }
    return {};
  }

  void Flush() { orders.clear(); }

  void Pop() { orders.pop_back(); }
};

using BuyOrders = Orders<order::Side::Buy>;
using SellOrders = Orders<order::Side::Sell>;

}// namespace order_book::core
