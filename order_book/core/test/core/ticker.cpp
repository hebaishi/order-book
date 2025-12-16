#include <catch2/catch_test_macros.hpp>

#include <order_book/core/ticker.hpp>
#include <sstream>

namespace order_book::core {

TEST_CASE("An empty ticker", "[ticker]")
{
  Ticker ticker{};
  SECTION("streams as an empty string")
  {
    std::stringstream sstr;
    sstr << ticker;
    CHECK(sstr.str().empty());
  }

  SECTION("returns true for IsEmpty") { CHECK(ticker.IsEmpty()); }
  SECTION("compares true with an empty string") { CHECK(ticker == ""); }
  SECTION("compares false with an non-empty string") { CHECK(ticker != "AABB"); }
  SECTION("compares smaller to a non-empty Ticker") { CHECK(ticker < Ticker{ "AABB" }); }
  SECTION("can be compared to a string_view")
  {
    CHECK(ticker == std::string_view());
    CHECK(ticker != std::string_view("AABB"));
  }
}
TEST_CASE("Ticker constructed from a string_view", "[ticker]")
{
  Ticker ticker{ "AABB" };
  SECTION("streams contents correctly")
  {
    std::stringstream sstr;
    sstr << ticker;
    CHECK(sstr.str() == "AABB");
  }
  SECTION("returns false for IsEmpty") { CHECK(!ticker.IsEmpty()); }
  SECTION("compares equal with the same string") { CHECK(ticker == Ticker{ "AABB" }); }
  SECTION("compares not equal with a different string") { CHECK(ticker != Ticker{ "AABC" }); }
  SECTION("compares smaller/greater correctly")
  {
    CHECK(ticker > Ticker{ "AAAA" });
    CHECK(ticker < Ticker{ "BBBB" });
  }
  SECTION("can be compared to a string view")
  {
    CHECK(ticker == std::string_view("AABB"));
    CHECK(ticker != std::string_view());
  }
}

TEST_CASE("Ticker only constructs with the right size string", "[ticker]")
{
  CHECK_THROWS(Ticker{ "A" });
  CHECK_THROWS(Ticker{ "AA" });
  CHECK_THROWS(Ticker{ "AAA" });
  CHECK_NOTHROW(Ticker{ "AAAA" });
}

}// namespace order_book::core
