#include "allocator.hpp"

#include <gtest/gtest.h>

#include <memory>
#include <vector>

using namespace JefLib::Allocator;

struct AllocatorTest : testing::Test {
  std::unique_ptr<Allocator<int>> alloc = std::make_unique<Allocator<int>>();
};

TEST_F(AllocatorTest, AllocDealoc) {
  int* iptr = alloc->allocate(4);
  for (auto i = 0ull; i < 4; ++i) {
    iptr[i] = i;
  }

  auto res{true};
  for (auto i = 0ull; i < 4; ++i) {
    res &= (iptr[i] == static_cast<int>(i));
  }

  alloc->deallocate(iptr);

  EXPECT_TRUE(res);
}

TEST_F(AllocatorTest, ConstexprAllocDealoc) {
  constexpr auto res = []() {
    Allocator<int> al;
    int* iptr = al.allocate(1);
    *iptr = 42;
    auto ret = *iptr;
    al.deallocate(iptr);
    return ret;
  }();

  static_assert(res == 42);
  EXPECT_TRUE(true);
}

TEST_F(AllocatorTest, ContainerAlloc) {
  std::vector<int> v{1, 2, 3, 4};
  std::vector<int, Allocator<int>> valloc{1, 2, 3, 4};
  auto res{true};

  for (std::size_t i{}; const auto &it : valloc) {
    res &= v[i++] == it;
  }

  EXPECT_TRUE(res && valloc.size() == 4);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
