#include <order_book/core/message/new_order.hpp>
#include <order_book/core/message/serialize.hpp>
#include <order_book/core/order/side.hpp>
#include <string>
namespace order_book::core::message {


std::string Serialize(const NewOrder &new_order)
{
  std::string serialized = "N,";
  serialized += std::to_string(new_order.user_id);
  serialized += ",";
  serialized += new_order.symbol.ToString();
  serialized += ",";
  serialized += std::to_string(new_order.price);
  serialized += ",";
  serialized += std::to_string(new_order.quantity);
  serialized += ",";
  serialized += new_order.side == order::Side::Buy ? "B" : "S";
  return serialized;
}
}// namespace order_book::core::message
