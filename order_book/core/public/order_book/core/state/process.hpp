#pragma once
#include <order_book/core/message/new_order.hpp>
#include <order_book/core/state.hpp>
#include <core_export.h>
namespace order_book::core::state {
order::Id CORE_EXPORT Process(State &state, const message::NewOrder &new_order);
}
