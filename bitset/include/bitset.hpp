#ifndef JEFLIB_BITSET
#define JEFLIB_BITSET

#include <array>
#include <bitset>
#include <iostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>

namespace JefLib {
namespace Bitset {

template <std::size_t N>
struct Is_Trivially_Constructible {
  static constexpr auto value = (N <= 8 * sizeof(uint64_t));
};

template <typename Param>
constexpr std::uint64_t sum(const Param& param) {
  return param;
}

template <typename P1, typename... Params>
constexpr std::uint64_t sum(const P1& p1, const Params&... params) {
  return p1 + sum(params...);
}

template <typename Bitset, std::size_t Size = Bitset().size(),
          typename std::enable_if<Is_Trivially_Constructible<Size>::value,
                                  int>::type = 0>
std::uint64_t to_uint64(const Bitset& bs) {
  std::uint64_t result{};
  for (std::uint64_t i = 0; i < Size; ++i) {
    result |= (static_cast<std::uint64_t>(bs[i]) << i);
  }
  return result;
}

template <typename... Bitsets, std::size_t Size = sum(Bitsets().size()...),
          std::size_t Num_Params = sizeof...(Bitsets),
          typename std::enable_if<Is_Trivially_Constructible<Size>::value,
                                  int>::type = 0>
std::bitset<Size> concat(const Bitsets&... bitsets) {
  const auto list_ulong =
      std::array<std::uint64_t, Num_Params>{to_uint64(bitsets)...};
  constexpr auto List_Sizes =
      std::array<std::uint64_t, Num_Params>{Bitsets().size()...};
  std::uint64_t bs_uint{};
  std::size_t idx{};
  std::size_t shift{Size};
  for (auto bs = list_ulong.rbegin(); bs != list_ulong.rend(); ++bs) {
    bs_uint |= (*bs << (shift -= List_Sizes[idx++]));
  }
  return std::bitset<Size>{bs_uint};
}

template <typename... Bitsets, std::size_t Size = sum(Bitsets().size()...),
          std::size_t Num_Params = sizeof...(Bitsets),
          typename std::enable_if<!Is_Trivially_Constructible<Size>::value,
                                  int>::type = 0>
std::bitset<Size> concat(const Bitsets&... bitsets) {
  const auto list_string =
      std::array<std::string, Num_Params>{bitsets.to_string()...};
  std::string bs_str;
  for (const auto& bs : list_string) {
    bs_str += bs;
  }
  return std::bitset<Size>{bs_str};
}

template <typename Bitset, typename BS1>
void split(const Bitset& bs, BS1& bs1) {
  static_assert(Bitset().size() == BS1().size(), "Bitset sizes mismatch");
  bs1 = BS1{bs};
}

template <typename Bitset, typename BS1, typename... Bitsets,
          std::uint64_t Bs_Size = Bitset().size(),
          std::uint64_t Bs1_Size = BS1().size(),
          std::uint64_t Bsrest_Size = sum(Bitsets().size()...),
          typename std::enable_if<Is_Trivially_Constructible<Bs_Size>::value,
                                  int>::type = 0>
void split(const Bitset& bs, BS1& bs1, Bitsets&... bitsets) {
  static_assert(Bs_Size == Bs1_Size + Bsrest_Size, "Bitset sizes mismatch");
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
          std::uint64_t Bsrest_Size = sum(Bitsets().size()...),
          typename std::enable_if<!Is_Trivially_Constructible<Bs_Size>::value,
                                  int>::type = 0>
void split(const Bitset& bs, BS1& bs1, Bitsets&... bitsets) {
  static_assert(Bs_Size == Bs1_Size + Bsrest_Size, "Bitset sizes mismatch");
  bs1 = BS1{bs.to_string().substr(0, Bs1_Size)};
  const auto& s2 = std::bitset<Bs_Size - Bs1_Size>{
      bs.to_string().substr(Bs1_Size, Bs_Size - Bs1_Size)};
  split(s2, bitsets...);
}

template <typename Bitset, std::size_t Offset, std::size_t Count,
          std::size_t Bs_Size = Bitset().size(),
          typename std::enable_if<Is_Trivially_Constructible<Bs_Size>::value,
                                  int>::type = 0>
std::bitset<Count> range(const Bitset& bs) {
  std::uint64_t bs_uint{};

  for (auto idx = Offset; idx < Offset + Count; ++idx) {
    bs_uint |= bs[idx] << (idx - Offset);
  }
  return std::bitset<Count>{bs_uint};
}

template <typename Bitset, std::size_t Bs_Size = Bitset().size(),
          typename std::enable_if<Is_Trivially_Constructible<Bs_Size>::value,
                                  int>::type = 0>
auto range(const Bitset& bs, const std::uint64_t offset,
           const std::uint64_t count) -> Bitset {
  std::uint64_t bs_uint{};

  for (auto idx = offset; idx < offset + count; ++idx) {
    bs_uint |= bs[idx] << (idx - offset);
  }
  return Bitset{bs_uint};
}

template <typename Bitset, std::size_t Bs_Size = Bitset().size(),
          typename std::enable_if<!Is_Trivially_Constructible<Bs_Size>::value,
                                  int>::type = 0>
auto range(const Bitset& bs, const std::uint64_t offset,
           const std::uint64_t count) -> Bitset {
  return Bitset{bs.to_string().substr(Bs_Size - offset - count, count)};
}

}  // namespace Bitset
}  // namespace JefLib

#endif
