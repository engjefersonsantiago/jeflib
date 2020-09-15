#include "bitset.hpp"

#include <gtest/gtest.h>

#include <bitset>

using namespace JefLib::Bitset;

struct BitsetTest : testing::Test {};

TEST_F(BitsetTest, SumSizes) {
  std::bitset<1> bs1{};
  std::bitset<2> bs2{};
  std::bitset<3> bs3{};
  EXPECT_EQ(sum(bs1.size(), bs2.size(), bs3.size()), 6);
}

TEST_F(BitsetTest, Concat) {
  constexpr std::bitset<1> bs1{1};
  constexpr std::bitset<2> bs2{1};
  constexpr std::bitset<3> bs3{1};
  EXPECT_EQ(concat(bs1, bs2, bs3), std::bitset<6>("101001"));
}

TEST_F(BitsetTest, ConcatConstexpr) {
  constexpr std::bitset<1> bs1{1};
  constexpr std::bitset<2> bs2{1};
  constexpr std::bitset<3> bs3{1};
  static_assert(to_uint64(concat(bs1, bs2, bs3)) ==
                to_uint64(std::bitset<6>(41)));
}

TEST_F(BitsetTest, Split) {
  std::bitset<1> bs1{};
  std::bitset<2> bs2{};
  std::bitset<3> bs3{};
  split(std::bitset<6>("110100"), bs1, bs2, bs3);
  EXPECT_TRUE(bs1 == 1 && bs2 == 2 && bs3 == 4);
}

TEST_F(BitsetTest, ConcatSplit) {
  std::bitset<1> bs1{};
  std::bitset<2> bs2{};
  std::bitset<3> bs3{};
  constexpr std::bitset<1> bs1_golden{1};
  constexpr std::bitset<2> bs2_golden{1};
  constexpr std::bitset<3> bs3_golden{1};
  constexpr auto concat_result = concat(bs1_golden, bs2_golden, bs3_golden);
  split(concat_result, bs1, bs2, bs3);
  EXPECT_TRUE(bs1 == bs1_golden && bs2 == bs2_golden && bs3 == bs3_golden);
}

TEST_F(BitsetTest, ConcatStr) {
  constexpr std::bitset<64> bs1{1};
  constexpr std::bitset<1> bs2{1};
  constexpr auto golden = std::bitset<65>{3};
  EXPECT_EQ(concat(bs1, bs2), golden);
}

TEST_F(BitsetTest, SplittStr) {
  constexpr auto golden = std::bitset<65>{3};
  std::bitset<64> bs1{};
  std::bitset<1> bs2{};
  split(golden, bs1, bs2);
  EXPECT_TRUE(bs1 == 1 && bs2 == 1);
}

TEST_F(BitsetTest, ConcatSplitStr) {
  std::bitset<64> bs1{};
  std::bitset<64> bs2{};
  constexpr std::bitset<64> bs1_golden{
      std::numeric_limits<std::uint64_t>::max()};
  constexpr std::bitset<64> bs2_golden{1};
  const auto concat_result = concat(bs1_golden, bs2_golden);
  split(concat_result, bs1, bs2);
  EXPECT_TRUE(bs1 == bs1_golden && bs2 == bs2_golden);
}

TEST_F(BitsetTest, ConcatSingleParam) {
  constexpr std::bitset<1> bs1{1};
  EXPECT_EQ(concat(bs1), bs1);
}

TEST_F(BitsetTest, SplitSingleParam) {
  constexpr std::bitset<1> bs1{1};
  std::bitset<1> bs2{};
  split(bs1, bs2);
  EXPECT_EQ(bs2, bs1);
}

TEST_F(BitsetTest, Range) {
  constexpr std::bitset<5> bs_golden{24};  // "11000"
  constexpr auto range_result = range(bs_golden, 3, 2);
  EXPECT_TRUE(range_result == std::bitset<5>{3});
}

TEST_F(BitsetTest, RangeFixed) {
  constexpr std::bitset<5> bs_golden{24};  // "11000"
  constexpr auto range_result = range<decltype(bs_golden), 3ul, 2ul>(bs_golden);
  EXPECT_TRUE(range_result == std::bitset<2>{3});
}

TEST_F(BitsetTest, RangeStr) {
  std::bitset<65> bs_golden{24};  // "11000"
  auto range_result = range(bs_golden, 3, 2);
  bs_golden = 1;
  auto range_result2 = range(bs_golden, 0, 1);
  bs_golden[64] = 1;
  auto range_result3 = range(bs_golden, 64, 1);
  EXPECT_TRUE(range_result == decltype(bs_golden){3} &&
              range_result2 == decltype(bs_golden){1} &&
              range_result3 == decltype(bs_golden){1});
}
