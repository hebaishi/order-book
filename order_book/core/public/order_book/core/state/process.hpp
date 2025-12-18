#pragma once
#include <order_book/core/message/new_order.hpp>
#include <order_book/core/state.hpp>
namespace order_book::core::state {
void Process(State &state, const message::NewOrder &new_order);
}
