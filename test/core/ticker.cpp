#include <catch2/catch_test_macros.hpp>

#include <order_book/core/ticker.hpp>

namespace order_book::core {

TEST_CASE("Ticker can be constructed from a string_view", "[ticker]") { REQUIRE_NOTHROW(Ticker{ "AABB" }); }

TEST_CASE("Ticker object can be equality-compared to another", "[ticker]")
{
  Ticker ticker_a{ "AABB" };
  Ticker ticker_b{ "AABB" };
  REQUIRE(ticker_a == ticker_b);
}

TEST_CASE("Ticker object can be compared to another to see which is greater", "[ticker]")
{
  Ticker ticker_a{ "AAAA" };
  Ticker ticker_b{ "BBBB" };
  REQUIRE(ticker_a < ticker_b);
}

TEST_CASE("Ticker can be compared to a string_view", "[ticker]")
{
  Ticker ticker_a{ "AABB" };
  std::string_view str("AABB");
  REQUIRE(ticker_a == str);
}

TEST_CASE("Ticker can be printed as a string", "[ticker]")
{
  Ticker ticker{ "ABCD" };
  std::stringstream sstr;
  sstr << ticker;
  REQUIRE(sstr.str() == "ABCD");
}

TEST_CASE("Default-constructed ticker equal to empty string", "[ticker]")
{
  Ticker ticker{};
  REQUIRE(ticker == "");
}

TEST_CASE("Default-constructed Ticker can be printed as a string", "[ticker]")
{
  Ticker ticker{};
  std::stringstream sstr;
  sstr << ticker;
  REQUIRE(sstr.str() == "");
}

TEST_CASE("Default-constructed Ticker is empty", "[ticker]")
{
  Ticker ticker{};
  REQUIRE(ticker.IsEmpty());
}

TEST_CASE("String-costructed Ticker is not empty", "[ticker]")
{
  Ticker ticker{ "AABB" };
  REQUIRE(!ticker.IsEmpty());
}

TEST_CASE("Ticker only constructs with the right size string", "[ticker]")
{
  REQUIRE_THROWS(Ticker{ "A" });
  REQUIRE_THROWS(Ticker{ "AA" });
  REQUIRE_THROWS(Ticker{ "AAA" });
  REQUIRE_NOTHROW(Ticker{ "AAAA" });
}

}// namespace order_book::core
