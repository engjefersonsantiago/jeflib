#ifndef JEFLIB_COMMON
#define JEFLIB_COMMON

#include <type_traits>

namespace JefLib::Common {

// Concepts
template <typename To, typename... From>
concept Is_Same_Decay = (std::is_same_v<std::decay_t<From>, To> && ...);

template <typename... Params>
concept Has_Plus = requires(Params... params) {
  {(params + ...)};
};

// Maybe constraint sum to integral and floating point types
// Or check if the + operator exists for the overloaded params
constexpr auto sum(
    const auto&... params) requires Has_Plus<decltype(params)...> {
  return (params + ...);
}

}  // namespace JefLib::Common

#endif
