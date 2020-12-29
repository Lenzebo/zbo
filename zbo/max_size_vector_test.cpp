#include "max_size_vector.h"

#include <gtest/gtest.h>

namespace zbo::test {

constexpr size_t MAX_SIZE = 10;
TEST(MaxSizeVector, DefaultConstruction)
{
    constexpr MaxSizeVector<int, MAX_SIZE> VECTOR{};
    ASSERT_EQ(VECTOR.size(), 0);
    ASSERT_TRUE(VECTOR.empty());
    ASSERT_EQ(VECTOR.capacity(), MAX_SIZE);
}

TEST(MaxSizeVector, ConstructInitializerList)
{
    const MaxSizeVector<int, MAX_SIZE> vector{1, 2, 3, 4, 5};

    ASSERT_EQ(vector.size(), 5);
    ASSERT_EQ(vector.at(0), 1);
    ASSERT_EQ(vector.at(4), 5);
}

TEST(MaxSizeVector, ConstructAssignFromArray)
{
    constexpr std::array<int, 5> ARR{5, 4, 3, 2, 1};
    MaxSizeVector<int, MAX_SIZE> vector{ARR};
    ASSERT_TRUE(std::equal(ARR.begin(), ARR.end(), vector.begin(), vector.end()));
    vector = ARR;
    ASSERT_TRUE(std::equal(ARR.begin(), ARR.end(), vector.begin(), vector.end()));
}

TEST(MaxSizeVector, ConstructAssignFromVector)
{
    std::vector<int> arr{5, 4, 3, 2, 1};  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    MaxSizeVector<int, MAX_SIZE> vector{arr};
    ASSERT_TRUE(std::equal(arr.begin(), arr.end(), vector.begin(), vector.end()));
    vector = arr;
    ASSERT_TRUE(std::equal(arr.begin(), arr.end(), vector.begin(), vector.end()));
}

TEST(MaxSizeVector, PushPopBack)
{
    MaxSizeVector<int, MAX_SIZE> vector{1, 2, 3, 4, 5};  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    ASSERT_EQ(vector.front(), 1);
    ASSERT_EQ(vector.back(), 5);
    vector.push_back(6);  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    ASSERT_EQ(vector.back(), 6);
    vector.pop_back();
    ASSERT_EQ(vector.back(), 5);
    vector.pop_back();
    ASSERT_EQ(vector.back(), 4);
    vector.push_back(5);  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    ASSERT_EQ(vector.size(), 5);
}

TEST(MaxSizeVector, InsertErase)
{
    MaxSizeVector<int, MAX_SIZE> vector{};
    constexpr std::array<int, 5> ARR{5, 4, 3, 2, 1};
    vector.insert(vector.begin(), ARR.begin(), ARR.end());
    ASSERT_TRUE(std::equal(ARR.begin(), ARR.end(), vector.begin(), vector.end()));

    vector.erase(vector.begin());
    ASSERT_EQ(vector.front(), 4);

    vector.erase(vector.begin(), vector.end());
    ASSERT_TRUE(vector.empty());
    vector.insert(vector.begin(), ARR.begin(), ARR.end());
    vector.insert(vector.begin(), ARR.begin(), ARR.end());
    ASSERT_EQ(vector.size(), 2 * ARR.size());

    vector.erase(std::next(vector.begin()), std::prev(vector.end()));
    ASSERT_EQ(vector.size(), 2);
    ASSERT_EQ(vector.front(), 5);
    ASSERT_EQ(vector.back(), 1);
}

template <typename T, typename Func>
void testAlgorithm(std::vector<T> data, Func&& function)
{
    MaxSizeVector<T, MAX_SIZE> vec{data};
    std::invoke(function, data.begin(), data.end());
    std::invoke(function, vec.begin(), vec.end());
    ASSERT_TRUE(std::equal(data.begin(), data.end(), vec.begin(), vec.end()));
}

// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define TEST_ALGORITHM(data, function, ...)                                      \
    {                                                                            \
        MaxSizeVector<T, MAX_SIZE> vec{data};                                    \
        auto cpy = data;                                                         \
        function(cpy.begin(), cpy.end() __VA_OPT__(, ) __VA_ARGS__);             \
        function(vec.begin(), vec.end() __VA_OPT__(, ) __VA_ARGS__);             \
        ASSERT_TRUE(std::equal(cpy.begin(), cpy.end(), vec.begin(), vec.end())); \
    }

template <typename T>
void testAlgorithmsAgainstVector(const std::vector<T> data)
{
    TEST_ALGORITHM(data, std::sort);
    TEST_ALGORITHM(data, std::reverse);
    TEST_ALGORITHM(data, std::partition, [](int val) { return val > 5; });
    TEST_ALGORITHM(data, std::stable_partition, [](int val) { return val > 5; });
    TEST_ALGORITHM(data, std::remove_if, [](int val) { return val > 5; });
}

TEST(MaxSizeVector, Algorithms)
{
    testAlgorithmsAgainstVector<int>({});
    testAlgorithmsAgainstVector<int>({1, 7, 99, 3, 5, 28, 0, 1, 1});  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    testAlgorithmsAgainstVector<int>({1, 2, 3, 4, 5, 6});             // NOLINT(cppcoreguidelines-avoid-magic-numbers)
}

}  // namespace zbo::test