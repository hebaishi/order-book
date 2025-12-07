#pragma once
#include <array>
#include <stdexcept>
#include <string_view>

namespace order_book::core {

template<size_t N = 4> class Ticker
{
public:
  explicit Ticker() = default;
  explicit Ticker(std::string_view str)
  {
    if (str.size() != N) { throw std::runtime_error("Incorrect string size"); }
    std::copy(str.begin(), str.end(), storage_.begin());
  }

  [[nodiscard]] bool IsEmpty() const { return storage_[0] == '\0'; }

  bool operator==(const Ticker &) const = default;

  bool operator==(std::string_view str)
  {
    if (IsEmpty()) { return str.empty(); }
    return std::equal(storage_.begin(), storage_.end(), str.begin(), str.end());
  }
  bool operator<(const Ticker &other) const { return storage_ < other.storage_; }
  bool operator>(const Ticker &other) const { return storage_ > other.storage_; }
  friend std::ostream &operator<<(std::ostream &stream, const Ticker &ticker)
  {
    if (ticker.IsEmpty()) { return stream; }
    std::array<char, N + 1> string_array{};
    std::copy(ticker.storage_.begin(), ticker.storage_.end(), string_array.begin());
    stream << string_array.data();
    return stream;
  }

private:
  std::array<char, N> storage_;
};
}// namespace order_book::core
