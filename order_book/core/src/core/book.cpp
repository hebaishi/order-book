#include <order_book/core/book.hpp>
#include <order_book/core/order/symbol.hpp>

namespace order_book::core::order {

Book::Book(Symbol symbol) : symbol(symbol) {}

}// namespace order_book::core::order
