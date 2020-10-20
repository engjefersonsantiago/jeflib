#include "common.hpp"

#include <gtest/gtest.h>

#include <string>

using namespace std::literals;
using namespace JefLib::Common;

struct CommonTest : testing::Test {};

struct SCommonTest {
  std::size_t i{}, j{};
  constexpr auto operator+(const SCommonTest& other) const {
    return SCommonTest{i + other.i, j + other.j};
  }
  constexpr auto operator<=>(const SCommonTest&) const = default;
};

TEST_F(CommonTest, Sum) {
  // Constexpr trivial sum
  static_assert(sum(1, 2, 3) == 6);
  // Constexpr non-trivial sum
  static_assert(sum(SCommonTest{}, SCommonTest{1, 2}) == SCommonTest{1, 2});
  // non-trivial sum
  EXPECT_TRUE(sum("C++"s, "20"s) == "C++20"s && sum("C++20"s) == "C++20");
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
