#include "list.hpp"

#include <gtest/gtest.h>

TEST(ListTest, Empty) {
    EXPECT_TRUE(jtl::list<int>{}.size() == 0 && jtl::list<int>{}.empty());
}

TEST(ListTest, PushBack) {
    jtl::list<int> l;
    l.push_back(1);
    EXPECT_EQ(l.size(), 1);
    EXPECT_TRUE(!l.empty());
}

TEST(ListTest, FrontBack) {
    jtl::list<int> l;
    l.push_back(42);
    EXPECT_EQ(l.back(), l.front());
}

TEST(ListTest, PopBack) {
    jtl::list<int> l;
    l.push_back(42);
    l.pop_back();
    EXPECT_TRUE(l.empty());
}
