#ifndef JEFLIB_BITSET
#define JEFLIB_BITSET

#include <array>
#include <bitset>
#include <iostream>
#include <stdexcept>
#include <string>
#include <tuple>

namespace JefLib {
namespace Bitset {

template <std::size_t N>
concept Is_Trivially_Constructible = (N <= 8 * sizeof(uint64_t));

template <typename... Params>
constexpr auto sum(const Params&... params) {
  std::size_t val = 0;
  ((val += params), ...);
  return val;
}

template <typename Bitset, std::size_t Size = Bitset().size()>
constexpr auto to_uint64(const Bitset& bs) requires(
    Is_Trivially_Constructible<Size>) {
  std::uint64_t result{};
  for (std::uint64_t i = 0; i < Size; ++i) {
    result |= (static_cast<std::uint64_t>(bs[i]) << i);
  }
  return result;
}

template <typename... Bitsets, std::size_t Size = sum(Bitsets().size()...)>
constexpr auto concat(const Bitsets&... bitsets) requires(
    Is_Trivially_Constructible<Size>) {
  const auto list_ulong = std::array{to_uint64(bitsets)...};
  constexpr auto List_Sizes = std::array{Bitsets().size()...};
  std::uint64_t bs_uint{};
  std::size_t idx{};
  std::size_t shift{Size};
  for (auto bs = list_ulong.rbegin(); bs != list_ulong.rend(); ++bs) {
    bs_uint |= (*bs << (shift -= List_Sizes[idx++]));
  }
  return std::bitset<Size>{bs_uint};
}

template <typename... Bitsets, std::size_t Size = sum(Bitsets().size()...)>
auto concat(const Bitsets&... bitsets) requires(
    !Is_Trivially_Constructible<Size>) {
  const auto list_string = std::array{bitsets.to_string()...};
  std::string bs_str;
  for (const auto& bs : list_string) {
    bs_str += bs;
  }
  return std::bitset<Size>{bs_str};
}

template <typename Bitset, typename BS1>
auto split(const Bitset& bs, BS1& bs1) {
  static_assert(Bitset().size() == BS1().size());
  bs1 = BS1{bs};
}

template <typename Bitset, typename BS1, typename... Bitsets,
          std::uint64_t Bs_Size = Bitset().size(),
          std::uint64_t Bs1_Size = BS1().size(),
          std::uint64_t Bsrest_Size = sum(Bitsets().size()...)>
auto split(const Bitset& bs, BS1& bs1,
           Bitsets&... bitsets) requires(Is_Trivially_Constructible<Bs_Size>) {
  static_assert(Bs_Size == Bs1_Size + Bsrest_Size);
  bs1 = BS1{(bs >> (Bs_Size - Bs1_Size)).to_ulong()};
  constexpr auto Shift = ((Bs_Size - Bs1_Size) < 8 * sizeof(std::uint64_t))
                             ? (Bs_Size - Bs1_Size)
                             : 0;
  constexpr auto Mask =
      static_cast<std::uint64_t>((static_cast<std::uint64_t>(1) << Shift) - 1);
  const auto& s2 =
      std::bitset<Bs_Size - Bs1_Size>{(bs & Bitset{Mask}).to_ulong()};
  split(s2, bitsets...);
}

template <typename Bitset, typename BS1, typename... Bitsets,
          std::uint64_t Bs_Size = Bitset().size(),
          std::uint64_t Bs1_Size = BS1().size(),
          std::uint64_t Bsrest_Size = sum(Bitsets().size()...)>
auto split(const Bitset& bs, BS1& bs1,
           Bitsets&... bitsets) requires(!Is_Trivially_Constructible<Bs_Size>) {
  static_assert(Bs_Size == Bs1_Size + Bsrest_Size);
  bs1 = BS1{bs.to_string().substr(0, Bs1_Size)};
  const auto& s2 = std::bitset<Bs_Size - Bs1_Size>{
      bs.to_string().substr(Bs1_Size, Bs_Size - Bs1_Size)};
  split(s2, bitsets...);
}

template <typename Bitset, std::size_t Offset, std::size_t Count,
          std::size_t Bs_Size = Bitset().size()>
constexpr auto range(const Bitset& bs) requires(
    Is_Trivially_Constructible<Bs_Size> && (Offset + Count) <= Bs_Size) {
  std::uint64_t bs_uint{};

  for (auto idx = Offset; idx < Offset + Count; ++idx) {
    bs_uint |= bs[idx] << (idx - Offset);
  }
  return std::bitset<Count>{bs_uint};
}

template <typename Bitset, std::size_t Bs_Size = Bitset().size()>
constexpr auto range(
    const Bitset& bs, const std::uint64_t offset,
    const std::uint64_t count) requires(Is_Trivially_Constructible<Bs_Size>) {
  std::uint64_t bs_uint{};

  for (auto idx = offset; idx < offset + count; ++idx) {
    bs_uint |= bs[idx] << (idx - offset);
  }
  return Bitset{bs_uint};
}

template <typename Bitset, std::size_t Bs_Size = Bitset().size()>
auto range(
    const Bitset& bs, const std::uint64_t offset,
    const std::uint64_t count) requires(!Is_Trivially_Constructible<Bs_Size>) {
  return Bitset{bs.to_string().substr(Bs_Size - offset - count, count)};
}

}  // namespace Bitset
}  // namespace JefLib

#endif
