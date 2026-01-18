#include <algorithm>
#include <order_book/core/message/new_order.hpp>
#include <order_book/core/order/book.hpp>
#include <order_book/core/order/id.hpp>
#include <order_book/core/order/side.hpp>
#include <order_book/core/state.hpp>
#include <order_book/core/state/process.hpp>

namespace order_book::core::state {

order::Id AddOrder(order::Book &book, const message::NewOrder &new_order)
{
  order::Id order_id{};
  if (new_order.side == core::order::Side::Buy) {
    order_id = book.buy_orders.Add(new_order);
  } else {
    order_id = book.sell_orders.Add(new_order);
  }
  return order_id;
}

order::Id Process(State &state, const message::NewOrder &new_order)
{
  auto &books = state.books;
  auto existing_book = std::ranges::find_if(books, [&](const auto &book) { return book.symbol == new_order.symbol; });
  if (existing_book == books.end()) {
    books.emplace_back(new_order.symbol);
    return AddOrder(books.back(), new_order);
  } else {
    return AddOrder(*existing_book, new_order);
  }
}

}// namespace order_book::core::state
