#ifndef JEFLIB_ALLOCATOR
#define JEFLIB_ALLOCATOR

/**
 * @brief STL complaint Allocator
 * As std::allocator, this one is stateless
 * It's implemented using C++20 and can be used in constexpr contexts
 */

#include <memory>
namespace JefLib::Allocator {

template <typename T>
class Allocator {
 public:
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using propagate_on_container_move_assignment = std::true_type;
  using is_always_equal = std::true_type;

  constexpr Allocator() noexcept = default;
  constexpr Allocator(const Allocator& other) noexcept = default;
  template <class U>
  [[maybe_unused]] constexpr Allocator([
      [maybe_unused]] const Allocator<U>& other) noexcept {}
  constexpr ~Allocator() = default;

  [[nodiscard]] constexpr T* allocate(size_type n) { return new T[n]{}; }
  constexpr void deallocate(T* p, [[maybe_unused]] size_type n = 0) {
    delete[] p;
  }
};

}  // namespace JefLib::Allocator

#endif
