#pragma once
#include <order_book/core/message/new_order.hpp>
#include <variant>
namespace order_book::core {
using Message = std::variant<message::NewOrder>;

}// namespace order_book::core
