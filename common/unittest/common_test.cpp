#include "common.hpp"

#include <gtest/gtest.h>

#include <string>

using namespace std::literals;

struct SCommonTest {
    std::size_t i{}, j{};
    constexpr auto operator+(const SCommonTest &other) const {
        return SCommonTest{i + other.i, j + other.j};
    }
    constexpr auto operator<=>(const SCommonTest &) const = default;
};

TEST(CommonTest, Sum) {
    // Constexpr trivial sum
    static_assert(jtl::sum(1, 2, 3) == 6);
    // Constexpr non-trivial sum
    static_assert(
            jtl::sum(SCommonTest{}, SCommonTest{1, 2}) == SCommonTest{1, 2});
    // non-trivial sum
    EXPECT_TRUE(jtl::sum("C++"s, "20"s) == "C++20"s &&
            jtl::sum("C++20"s) == "C++20");
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
