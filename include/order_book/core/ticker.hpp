#pragma once
#include <array>
#include <stdexcept>
#include <string_view>

template<size_t N = 4> class Ticker
{
public:
  explicit Ticker(std::string_view str)
  {
    if (str.size() > N) { throw std::runtime_error("String too big"); }
    std::copy(str.begin(), str.end(), storage_.begin());
  }
  bool operator==(const Ticker &) const = default;

  bool operator==(std::string_view str) { return std::equal(storage_.begin(), storage_.end(), str.begin(), str.end()); }
  bool operator<(const Ticker &other) const { return storage_ < other.storage_; }
  friend std::ostream &operator<<(std::ostream &stream, const Ticker &ticker)
  {
    std::array<char, N + 1> string_array;
    std::copy(ticker.storage_.begin(), ticker.storage_.end(), string_array.begin());
    stream << string_array.data();
    return stream;
  }

private:
  std::array<char, N> storage_;
};
