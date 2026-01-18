#pragma once
#include <order_book/core/order/books.hpp>
namespace order_book::core {
struct State
{
  order::Books books;
};
}// namespace order_book::core
