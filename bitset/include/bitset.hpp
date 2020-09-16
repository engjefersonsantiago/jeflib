#ifndef JEFLIB_BITSET
#define JEFLIB_BITSET

#include <array>
#include <bitset>
#include <iostream>
#include <stdexcept>
#include <string>
#include <tuple>

namespace JefLib::Bitset {

static constexpr auto UINT64_BIT_WIDTH = 8 * sizeof(uint64_t);
template <typename Bitset>
using rm_cvref_t = std::remove_cvref_t<Bitset>;

constexpr auto sum(const auto&... params) {
  std::size_t val = 0;
  ((val += params), ...);
  return val;
}

// Declared template variable cuz a concept does not compile in g++
template <typename... Bitset>
constexpr auto Is_Trivially_Constructible =
    sum(rm_cvref_t<Bitset>().size()...) <= UINT64_BIT_WIDTH;

template <std::size_t N>
[[maybe_unused]] static constexpr bool is_trivially_constructible_v =
    (N <= UINT64_BIT_WIDTH);

constexpr auto to_uint64(const auto& bs) requires(
    Is_Trivially_Constructible<decltype(bs)>) {
  auto Size = rm_cvref_t<decltype(bs)>().size();
  std::uint64_t result{};
  for (std::uint64_t i = 0; i < Size; ++i) {
    result |= (static_cast<std::uint64_t>(bs[i]) << i);
  }
  return result;
}

constexpr auto concat(const auto&... bitsets) requires(
    Is_Trivially_Constructible<decltype(bitsets)...>) {
  constexpr auto Size = sum(rm_cvref_t<decltype(bitsets)>().size()...);
  const auto list_ulong = std::array{to_uint64(bitsets)...};
  constexpr auto List_Sizes =
      std::array{rm_cvref_t<decltype(bitsets)>().size()...};
  std::uint64_t bs_uint{};
  std::size_t idx{};
  std::size_t shift{Size};
  for (auto bs = list_ulong.rbegin(); bs != list_ulong.rend(); ++bs) {
    bs_uint |= (*bs << (shift -= List_Sizes[idx++]));
  }
  return std::bitset<Size>{bs_uint};
}

auto concat(const auto&... bitsets) requires(
    !Is_Trivially_Constructible<decltype(bitsets)...>) {
  constexpr auto Size = sum(rm_cvref_t<decltype(bitsets)>().size()...);
  const auto list_string = std::array{bitsets.to_string()...};
  std::string bs_str;
  for (const auto& bs : list_string) {
    bs_str += bs;
  }
  return std::bitset<Size>{bs_str};
}

constexpr auto get_split_shift(const auto a, const auto b) {
  return (a - b) < UINT64_BIT_WIDTH ? (a - b) : 0;
}

auto split(const auto& bs, auto& bs1) {
  static_assert(rm_cvref_t<decltype(bs)>().size() ==
                rm_cvref_t<decltype(bs1)>().size());
  bs1 = rm_cvref_t<decltype(bs1)>{bs};
}

auto split(const auto& bs, auto& bs1, auto&... bitsets) {
  constexpr std::uint64_t Bs_Size = rm_cvref_t<decltype(bs)>().size();
  constexpr std::uint64_t Bs1_Size = rm_cvref_t<decltype(bs1)>().size();
  constexpr std::uint64_t Bsrest_Size =
      sum(rm_cvref_t<decltype(bitsets)>().size()...);
  static_assert(Bs_Size == Bs1_Size + Bsrest_Size);
  constexpr auto Use_Str =
      !is_trivially_constructible_v<rm_cvref_t<decltype(bs)>().size()>;
  constexpr auto Shift = Bs_Size - Bs1_Size;
  if constexpr (Use_Str) {
    bs1 = rm_cvref_t<decltype(bs1)>{bs.to_string().substr(0, Bs1_Size)};
    const auto& bs2 =
        std::bitset<Shift>{bs.to_string().substr(Bs1_Size, Shift)};
    split(bs2, bitsets...);
  } else {
    constexpr auto Shift_Uint = get_split_shift(Bs_Size, Bs1_Size);
    constexpr auto Mask = (static_cast<std::uint64_t>(1) << Shift_Uint) - 1;
    bs1 = rm_cvref_t<decltype(bs1)>{(bs >> Shift).to_ulong()};
    const auto& bs2 =
        std::bitset<Shift>{(bs & rm_cvref_t<decltype(bs)>{Mask}).to_ulong()};
    split(bs2, bitsets...);
  }
}

template <std::size_t Offset, std::size_t Count>
constexpr auto range(const auto& bs) requires(
    Is_Trivially_Constructible<decltype(bs)> &&
    (Offset + Count) <= rm_cvref_t<decltype(bs)>().size()) {
  std::uint64_t bs_uint{};
  for (auto idx = Offset; idx < Offset + Count; ++idx) {
    bs_uint |= bs[idx] << (idx - Offset);
  }
  return std::bitset<Count>{bs_uint};
}

constexpr auto range(
    const auto& bs, const std::uint64_t offset,
    const std::uint64_t
        count) requires(Is_Trivially_Constructible<decltype(bs)>) {
  std::uint64_t bs_uint{};
  for (auto idx = offset; idx < offset + count; ++idx) {
    bs_uint |= bs[idx] << (idx - offset);
  }
  return rm_cvref_t<decltype(bs)>{bs_uint};
}

auto range(const auto& bs, const std::uint64_t offset,
           const std::uint64_t
               count) requires(!Is_Trivially_Constructible<decltype(bs)>) {
  constexpr auto Bs_Size = rm_cvref_t<decltype(bs)>().size();
  return rm_cvref_t<decltype(bs)>{
      bs.to_string().substr(Bs_Size - offset - count, count)};
}

}  // namespace JefLib::Bitset

#endif
