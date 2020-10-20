#ifndef JEFLIB_ITERATOR
#define JEFLIB_ITERATOR

#include <iterator>
namespace JefLib::Iterator {

template <typename T>
class RandomAccessIterator {
 public:
  //using iterator_category = std::random_access_iterator_tag;
  using iterator_category = std::contiguous_iterator_tag;
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;

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

  explicit constexpr RandomAccessIterator(const pointer& ptr) : data_{ptr} {}
  explicit constexpr RandomAccessIterator(pointer&& ptr) : data_{ptr} {}

  // default operators
  constexpr RandomAccessIterator() = default;
  constexpr RandomAccessIterator(const RandomAccessIterator&) = default;
  constexpr RandomAccessIterator(RandomAccessIterator&&) = default;
  constexpr RandomAccessIterator& operator=(const RandomAccessIterator&) =
      default;
  constexpr RandomAccessIterator& operator=(RandomAccessIterator&&) = default;
  constexpr RandomAccessIterator& operator<=>(
      const RandomAccessIterator&) const = default;

 private:
  pointer data_{nullptr};
};

template <typename T>
constexpr auto operator-(
    const RandomAccessIterator<T>& lhs,
    const RandomAccessIterator<T>& rhs) {
  return lhs.operator->() - rhs.operator->();
}

}  // namespace JefLib::Iterator
#endif
