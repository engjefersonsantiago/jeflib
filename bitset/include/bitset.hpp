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
using rm_cvref_t = std::remove_reference_t<std::remove_cv_t<Bitset>>;

template <typename... Params>
constexpr auto sum(const Params... params) {
  std::size_t val = 0;
  ((val += params), ...);
  return val;
}

template <typename... Bitset>
[[maybe_unused]] static constexpr auto is_trivially_constructible_v =
    sum(Bitset().size()...) <= UINT64_BIT_WIDTH;

template <typename Bitset, typename std::enable_if_t<
                               is_trivially_constructible_v<Bitset>, int> = 0>
constexpr auto to_uint64(const Bitset& bs) {
  auto Size = Bitset().size();
  std::uint64_t result{};
  for (std::uint64_t i = 0; i < Size; ++i) {
    result |= (static_cast<std::uint64_t>(bs[i]) << i);
  }
  return result;
}

template <typename... Bitsets,
          typename std::enable_if_t<is_trivially_constructible_v<Bitsets...>,
                                    int> = 0>
constexpr auto concat(const Bitsets&... bitsets) {
  constexpr auto Size = sum(Bitsets().size()...);
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

template <typename... Bitsets,
          typename std::enable_if_t<!is_trivially_constructible_v<Bitsets...>,
                                    int> = 0>
auto concat(const Bitsets&... bitsets) {
  constexpr auto Size = sum(Bitsets().size()...);
  const auto list_string = std::array{bitsets.to_string()...};
  std::string bs_str;
  for (const auto& bs : list_string) {
    bs_str += bs;
  }
  return std::bitset<Size>{bs_str};
}

constexpr auto get_split_shift(const std::size_t a, std::size_t b) {
  return (a - b) < UINT64_BIT_WIDTH ? (a - b) : 0;
}

template <typename BS1, typename BS2>
auto split(const BS1& bs, BS2& bs1) {
  static_assert(BS1().size() == BS2().size());
  bs1 = rm_cvref_t<decltype(bs1)>{bs};
}

template <typename BS, typename BS1, typename... Bitsets>
auto split(const BS& bs, BS1& bs1, Bitsets&... bitsets) {
  constexpr std::uint64_t Bs_Size = BS().size();
  constexpr std::uint64_t Bs1_Size = BS1().size();
  constexpr std::uint64_t Bsrest_Size = sum(Bitsets().size()...);
  static_assert(Bs_Size == Bs1_Size + Bsrest_Size);
  constexpr auto Use_Str = !is_trivially_constructible_v<BS>;
  constexpr auto Shift = Bs_Size - Bs1_Size;
  if constexpr (Use_Str) {
    bs1 = rm_cvref_t<decltype(bs1)>{bs.to_string().substr(0, Bs1_Size)};
    const auto& bs2 =
        std::bitset<Shift>{bs.to_string().substr(Bs1_Size, Shift)};
    split(bs2, bitsets...);
  } else {
    constexpr auto Shift_Uint = get_split_shift(Bs_Size, Bs1_Size);
    constexpr auto Mask = (static_cast<std::uint64_t>(1) << Shift_Uint) - 1;
    bs1 = BS1{(bs >> Shift).to_ulong()};
    const auto& bs2 = std::bitset<Shift>{(bs & BS{Mask}).to_ulong()};
    split(bs2, bitsets...);
  }
}

template <typename Bitset, std::size_t Offset, std::size_t Count,
          typename std::enable_if_t<is_trivially_constructible_v<Bitset> &&
                                        (Offset + Count) <= Bitset().size(),
                                    int> = 0>
constexpr auto range(const Bitset& bs) {
  std::uint64_t bs_uint{};
  for (auto idx = Offset; idx < Offset + Count; ++idx) {
    bs_uint |= bs[idx] << (idx - Offset);
  }
  return std::bitset<Count>{bs_uint};
}

template <typename Bitset, typename std::enable_if_t<
                               is_trivially_constructible_v<Bitset>, int> = 0>
constexpr auto range(const Bitset& bs, const std::uint64_t offset,
                     const std::uint64_t count) {
  std::uint64_t bs_uint{};
  for (auto idx = offset; idx < offset + count; ++idx) {
    bs_uint |= bs[idx] << (idx - offset);
  }
  return Bitset{bs_uint};
}

template <typename Bitset, typename std::enable_if_t<
                               !is_trivially_constructible_v<Bitset>, int> = 0>
auto range(const Bitset& bs, const std::uint64_t offset,
           const std::uint64_t count) {
  constexpr auto Bs_Size = Bitset().size();
  return Bitset{bs.to_string().substr(Bs_Size - offset - count, count)};
}

}  // namespace JefLib::Bitset

#endif
