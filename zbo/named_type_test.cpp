
#include "named_type.h"
#include "stream_container.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <numeric>

namespace zbo::test {
struct Type1 : public NamedType<int, Type1>, Comparable<Type1>
{
    using NamedType::NamedType;
};

using Type2 = NamedType<int, struct Type2Tag>;
static_assert(!std::is_same_v<Type1, Type2>, "Types with different tag should not be the same");

struct Meter : public NamedType<double, Meter>, Comparable<Meter>, Arithmetic<Meter>, Streamable<Meter>
{
    using NamedType::NamedType;
};

static_assert(std::is_same_v<UnderlyingType<Meter>, double>, "The underlying type of meter shall be double");

constexpr Meter M1{1.0};
constexpr Meter M2{1.0};

static_assert(std::is_same_v<decltype(M1 / M2), double>, "Dividing two units gives a dimensionless quantity");

TEST(NamedType, Creation)
{
    constexpr int VALUE = 255;
    Type1 type{VALUE};
    ASSERT_EQ(type.get(), VALUE);

    auto cpy = type;
    ASSERT_EQ(cpy.get(), VALUE);
    ASSERT_EQ(cpy, type);
}

TEST(NamedType, Arithmetic)
{
    constexpr Meter LENGTH{5.0};
    auto result = 2 * LENGTH + Meter{10.0};  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    ASSERT_DOUBLE_EQ(result.get(), 20.0);
    double ratio = result / LENGTH;
    ASSERT_DOUBLE_EQ(ratio, 4.0);
    result /= ratio;
    ASSERT_EQ(result, LENGTH);
    result -= result;
    ASSERT_EQ(result, Meter{});
}

TEST(NamedType, Sorting)
{
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
    std::vector<double> lengths{1.23, 5.7, 0.3, -1.0, -10, 245555.3, 0.3};
    std::vector<Meter> lengthsM;
    std::transform(lengths.begin(), lengths.end(), std::back_inserter(lengthsM), [](double val) { return Meter(val); });

    std::sort(lengths.begin(), lengths.end());
    std::sort(lengthsM.begin(), lengthsM.end());
    streamContainer(std::cout, lengths) << "\n";
    streamContainer(std::cout, lengthsM) << "\n";

    ASSERT_TRUE(std::equal(lengths.begin(), lengths.end(), lengthsM.begin(), lengthsM.end(),
                           [](double v1, Meter v2) { return v1 == v2.get(); }));

    std::sort(lengths.begin(), lengths.end(), std::greater_equal{});
    std::sort(lengthsM.begin(), lengthsM.end(), std::greater_equal{});
    streamContainer(std::cout, lengths) << "\n";
    streamContainer(std::cout, lengthsM) << "\n";

    ASSERT_TRUE(std::equal(lengths.begin(), lengths.end(), lengthsM.begin(), lengthsM.end(),
                           [](double v1, Meter v2) { return v1 == v2.get(); }));

    const auto totalLength = std::accumulate(lengths.begin(), lengths.end(), 0.0);
    const auto totalLengthM = std::accumulate(lengthsM.begin(), lengthsM.end(), Meter{});

    ASSERT_DOUBLE_EQ(totalLength, totalLengthM.get());
}

}  // namespace zbo::test