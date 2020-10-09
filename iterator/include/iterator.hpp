#ifndef JEFLIB_ITERATOR
#define JEFLIB_ITERATOR

#include <memory>

namespace JefLib::Iterator {

template <typename T, typename Distance = std::ptrdiff_t, typename Pointer = T*,
          typename Reference = T&>
class RandomAccessIterator {
 public:
  using value_type = T;
  using difference_type = Distance;
  using pointer = Pointer;
  using const_pointer = const Pointer;
  using reference = Reference;
  using const_reference = const Reference;

  constexpr auto& operator++() { return (++data_, *this); }
  constexpr auto operator++(int) {
    auto ret = *this;
    ++data_;
    return ret;
  }

  constexpr auto& operator+=(int inc) { return (data_ += inc, *this); }
  constexpr auto& operator-=(int inc) { return (data_ += (-inc), *this); }

  constexpr auto& operator--() { return (--data_, *this); }
  constexpr auto operator--(int) {
    auto ret = *this;
    --data_;
    return ret;
  }

  constexpr auto& operator*() { return *data_; }
  constexpr const auto& operator*() const { return *data_; }
  constexpr auto operator->() { return data_; }
  constexpr auto operator->() const { return data_; }

  constexpr auto operator-(const RandomAccessIterator& other) const {
    return std::distance(this->data_, other.operator->());
  }

  explicit RandomAccessIterator(const pointer& ptr) : data_{ptr} {}
  explicit RandomAccessIterator(pointer&& ptr) : data_{ptr} {}

  // default operators
  constexpr RandomAccessIterator() = default;
  constexpr RandomAccessIterator(const RandomAccessIterator&) = default;
  constexpr RandomAccessIterator(RandomAccessIterator&) = default;
  constexpr RandomAccessIterator(RandomAccessIterator&&) = default;
  constexpr RandomAccessIterator& operator=(const RandomAccessIterator&) =
      default;
  constexpr RandomAccessIterator& operator=(RandomAccessIterator&&) = default;
  constexpr RandomAccessIterator& operator<=>(
      const RandomAccessIterator&) const = default;

 private:
  pointer data_{nullptr};
};

}  // namespace JefLib::Iterator
#endif
