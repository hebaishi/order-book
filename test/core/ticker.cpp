#include <catch2/catch_test_macros.hpp>

#include <order_book/core/ticker.hpp>

namespace order_book::core {

TEST_CASE("An empty ticker", "[ticker]")
{
  Ticker ticker{};
  SECTION("streams as an empty string")
  {
    std::stringstream sstr;
    sstr << ticker;
    REQUIRE(sstr.str() == "");
  }

  SECTION("returns true for IsEmpty") { REQUIRE(ticker.IsEmpty()); }
  SECTION("compares true with an empty string") { REQUIRE(ticker == ""); }
  SECTION("compares false with an non-empty string") { REQUIRE(ticker != "AABB"); }
  SECTION("compares smaller to a non-empty Ticker") { REQUIRE(ticker < Ticker{ "AABB" }); }
  SECTION("can be compared to a string_view")
  {
    REQUIRE(ticker == std::string_view());
    REQUIRE(ticker != std::string_view("AABB"));
  }
}
TEST_CASE("Ticker constructed from a string_view", "[ticker]")
{
  Ticker ticker{ "AABB" };
  SECTION("streams contents correctly")
  {
    std::stringstream sstr;
    sstr << ticker;
    REQUIRE(sstr.str() == "AABB");
  }
  SECTION("returns false for IsEmpty") { REQUIRE(!ticker.IsEmpty()); }
  SECTION("compares equal with the same string") { REQUIRE(ticker == Ticker{ "AABB" }); }
  SECTION("compares not equal with a different string") { REQUIRE(ticker != Ticker{ "AABC" }); }
  SECTION("compares smaller/greater correctly")
  {
    REQUIRE(ticker > Ticker{ "AAAA" });
    REQUIRE(ticker < Ticker{ "BBBB" });
  }
  SECTION("can be compared to a string view")
  {
    REQUIRE(ticker == std::string_view("AABB"));
    REQUIRE(ticker != std::string_view());
  }
}

TEST_CASE("Ticker only constructs with the right size string", "[ticker]")
{
  REQUIRE_THROWS(Ticker{ "A" });
  REQUIRE_THROWS(Ticker{ "AA" });
  REQUIRE_THROWS(Ticker{ "AAA" });
  REQUIRE_NOTHROW(Ticker{ "AAAA" });
}

}// namespace order_book::core
