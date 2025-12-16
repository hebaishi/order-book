#include <boost/json/serialize.hpp>
#include <order_book/core/order.hpp>
#include <order_book/core/order/side.hpp>
#include <ostream>
#include <sstream>
#include <string>

namespace order_book::core {

std::ostream &operator<<(std::ostream &stream, const Order &order)
{
  namespace js = boost::json;
  std::stringstream symbol_name;
  symbol_name << order.symbol;
  stream << js::serialize(js::object{
    { "quantity", order.quantity },
    { "price", order.price },
    { "user_id", order.user_id },
    { "id", order.id.has_value() ? std::to_string(order.id.value()) : "null" },
    { "symbol", symbol_name.str() },
    { "side", order.side == order::Side::Buy ? "buy" : "sell" },
  });
  return stream;
}

}// namespace order_book::core
