#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/range/iterator_range_core.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <order_book/core/order/side.hpp>
#include <string>

#include <order_book/core/message/new_order.hpp>
namespace order_book::core::message {


TEST_CASE("NewOrder can be serialized and deserialized", "[messaging]")
{
  constexpr auto symbol = order::Symbol{ "GOLD" };
  const auto original_order =
    NewOrder{ .quantity = 100, .price = 122, .user_id = 2345, .symbol = symbol, .side = order::Side::Buy };
  std::string output;

  {
    auto stream = boost::iostreams::filtering_ostream(boost::iostreams::back_inserter(output));
    boost::archive::text_oarchive output_archive(stream);
    output_archive << original_order;
  }
  std::cout << output << '\n';

  NewOrder deserialized_order;
  {
    auto stream = boost::iostreams::filtering_istream(boost::make_iterator_range(output));
    boost::archive::text_iarchive input_archive(stream);
    input_archive >> deserialized_order;
  }
  REQUIRE(original_order == deserialized_order);
}
}// namespace order_book::core::message
