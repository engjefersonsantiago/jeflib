#include "common.hpp"

#include <gtest/gtest.h>

#include <string>

using namespace std::literals;
using namespace JefLib::Common;

struct CommonTest : testing::Test {};

TEST_F(CommonTest, Sum) {
  static_assert(sum(1, 2, 3) == 6);
  EXPECT_TRUE(sum("C++"s, "20"s) == "C++20"s && sum("C++20"s) == "C++20");
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
