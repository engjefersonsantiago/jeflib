#include "iterator.hpp"

#include <gtest/gtest.h>

#include <memory>

using namespace JefLib::Iterator;

struct IteratorTest : testing::Test {
  std::unique_ptr<RandomAccessIterator<int>> alloc = std::make_unique<RandomAccessIterator<int>>();
};

TEST_F(IteratorTest, IteratorConstruct) {
  EXPECT_TRUE(true);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


