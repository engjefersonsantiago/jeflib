#include "vector.hpp"

#include <gtest/gtest.h>

#include <string>

#include "allocator.hpp"
#include "common.hpp"
#include "iterator.hpp"

using namespace std::literals;

template <typename T>
struct S {
    T i;
    T j;
};

template <typename T>
using jtl_vector_t =
        jtl::vector<T, jtl::allocator<T>, jtl::RandomAccessIterator<T>>;

bool compare_vecs(const auto &vec, const auto &vec2) {
    auto res = vec.size() == vec2.size() && vec.capacity() == vec2.capacity();
    for (int i = 0; const auto &el : vec) {
        res &= vec2[i++] == el;
    }
    return res;
}

struct VectorTest : testing::Test {};

TEST_F(VectorTest, Size) {
    jtl_vector_t<int> vec{1, 2, 3, 4};
    jtl::vector vec2{1, 2, 3, 4};
    std::vector vec3{1, 2, 3, 4};
    EXPECT_TRUE(vec.size() == 4 && vec2.size() == 4 && vec3.size());
}

TEST_F(VectorTest, PushBack) {
    jtl_vector_t<int> vec{1, 2, 3, 4};
    vec.push_back(5);
    EXPECT_TRUE(vec.size() == 5);
}

TEST_F(VectorTest, EmplaceBack) {
    jtl_vector_t<S<int>> vec{{1, 2}, {3, 4}};
    vec.emplace_back(5, 6);
    EXPECT_TRUE(vec.size() == 3);
}

TEST_F(VectorTest, Iterators) {
    jtl_vector_t<int> vec{1, 2, 3, 4};
    std::vector vec2{1, 2, 3, 4};

    auto res{true};
    for (int i = 0; const auto &el : vec) {
        res &= vec2[i++] == el;
    }

    std::vector vec3{10, 20, 30, 40};
    for (int i = 0; auto &el : vec) {
        el = vec3[i++];
    }

    for (int i = 0; const auto &el : vec) {
        res &= vec3[i++] == el;
    }

    auto vec_it = vec.rbegin();
    auto vec3_it = vec3.rbegin();
    for (; vec_it != vec.rend() && vec3_it != vec3.rend();
            ++vec_it, ++vec3_it) {
        res &= (*vec3_it == *vec_it);
    }

    EXPECT_TRUE(res);
}

TEST_F(VectorTest, SizeResizeShrinkCapacity) {
    jtl_vector_t<int> vec;
    std::vector<int> vec2;

    vec.reserve(20);
    vec2.reserve(20);

    const auto compare_vecs = [](const auto &vec, const auto &vec2) {
        auto res =
                vec.size() == vec2.size() && vec.capacity() == vec2.capacity();
        if (res) {
            for (int i = 0; const auto &el : vec) {
                res &= vec2[i++] == el;
            }
        }
        return res;
    };

    auto res = compare_vecs(vec, vec2);

    vec.push_back(1);
    vec2.push_back(1);

    res &= compare_vecs(vec, vec2);

    vec.resize(200);
    vec2.resize(200);

    res &= compare_vecs(vec, vec2);

    vec.resize(20);
    vec2.resize(20);

    res &= compare_vecs(vec, vec2);

    vec.shrink_to_fit();
    vec2.shrink_to_fit();

    vec.emplace_back(10);
    vec2.emplace_back(10);

    res &= compare_vecs(vec, vec2);

    EXPECT_TRUE(res);
}

TEST_F(VectorTest, ClearEmpty) {
    jtl_vector_t<int> vec{1, 2, 3, 4};
    vec.clear();
    EXPECT_TRUE(vec.size() == 0 && vec.empty());
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
