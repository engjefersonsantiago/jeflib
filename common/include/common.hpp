#ifndef JEFLIB_COMMON
#define JEFLIB_COMMON

namespace JefLib::Common {

// Maybe constraint sum to integral and floating point types
// Or check if the + operator exists for the overloaded params
constexpr auto sum(const auto&... params) { return (params + ...); }

}  // namespace JefLib::Common

#endif
