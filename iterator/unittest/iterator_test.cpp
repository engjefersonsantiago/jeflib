#include "iterator.hpp"

#include <gtest/gtest.h>

#include <memory>

using namespace JefLib::Iterator;

struct IteratorTest : testing::Test {
  std::unique_ptr<RandomAccessIterator<int>> alloc =
      std::make_unique<RandomAccessIterator<int>>();
};

TEST_F(IteratorTest, IteratorConstruct) {
  std::vector v{1, 2, 3, 4};
  const decltype(v)::iterator vbit(v.begin().operator->());
  const decltype(v)::iterator veit(v.end().operator->());
  const RandomAccessIterator<int> cvbit(v.begin().operator->());
  const RandomAccessIterator<int> cveit(v.end().operator->());
  EXPECT_TRUE(std::distance(vbit, veit) == std::distance(cvbit, cveit));
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
