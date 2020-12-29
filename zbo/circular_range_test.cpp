#include "circular_range.h"

#include <gtest/gtest.h>

constexpr std::array<int, 4> ARRAY{1, 2, 3, 4};

TEST(CircularIterator, Forward)
{
    zbo::CircularRange<const int> range{ARRAY, 3};
    auto begin = range.begin();
    auto end = range.end();
    ASSERT_EQ(*begin, 4);
    ASSERT_EQ(*++begin, 1);
    ASSERT_EQ(*++begin, 2);
    ASSERT_EQ(*++begin, 3);
    ASSERT_EQ(++begin, end);
}

TEST(CircularIterator, Copy)
{
    zbo::CircularRange<const int> range{ARRAY, 3};
    std::vector<int> generated;
    std::copy(range.begin(), range.end(), std::back_inserter(generated));
    ASSERT_EQ(generated[0], 4);
    ASSERT_EQ(generated[1], 1);
    ASSERT_EQ(generated[2], 2);
    ASSERT_EQ(generated[3], 3);
}

TEST(CircularIterator, RandomAccess)
{
    zbo::CircularRange<const int> range{ARRAY, 3};
    ASSERT_EQ(range[0], 4);
    ASSERT_EQ(range[1], 1);
    ASSERT_EQ(range[2], 2);
    ASSERT_EQ(range[3], 3);

    ASSERT_EQ(std::distance(range.begin(), range.end()), ARRAY.size());
}