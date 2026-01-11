#pragma once
#include <order_book/core/order/book.hpp>
#include <order_book/core/trade.hpp>
#include <vector>

namespace order_book::core {
std::vector<Trade> Match(order::Book &order_book);
}
