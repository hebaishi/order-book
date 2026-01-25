#pragma once
#include <core_export.h>
#include <order_book/core/message.hpp>
#include <order_book/core/message/new_order.hpp>
namespace order_book::core::message {

CORE_EXPORT std::string Serialize(const message::NewOrder& new_order);

}// namespace order_book::core::message
