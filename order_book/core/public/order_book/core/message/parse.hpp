#pragma once
#include <core_export.h>
#include <optional>
#include <order_book/core/message.hpp>
#include <string_view>
namespace order_book::core::message {

CORE_EXPORT std::optional<order_book::core::Message> Parse(std::string_view buffer);

}// namespace order_book::core::message
