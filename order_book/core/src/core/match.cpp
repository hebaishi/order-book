#include <order_book/core/match.hpp>
namespace order_book::core {
std::vector<Trade> Match(order::Book &order_book)
{
  std::vector<Trade> trades;
  for (auto best_buy = order_book.buy_orders.GetBest(), best_sell = order_book.sell_orders.GetBest();
    best_buy && best_sell && best_buy->price >= best_sell->price;) {
    const auto trade_quantity = std::min(best_buy->quantity, best_sell->quantity);
    trades.emplace_back(Trade{ .buy_id = best_buy->id.value(),
      .sell_id = best_sell->id.value(),
      .quantity = trade_quantity,
      .price = best_sell->price });

    best_buy->quantity -= trade_quantity;
    best_sell->quantity -= trade_quantity;

    if (best_buy->quantity == 0) {
      order_book.buy_orders.Pop();
      best_buy = order_book.buy_orders.GetBest();
    }
    if (best_sell->quantity == 0) {
      order_book.sell_orders.Pop();
      best_sell = order_book.sell_orders.GetBest();
    }
  }

  return trades;
}
}// namespace order_book::core
