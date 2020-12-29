#include "meta_enum.h"
#include "meta_enum_iterator.h"

#include <gtest/gtest.h>

ZBO_ENUM(MyTestEnum, int, ONE, TWO, THREE)

namespace test_namespace {

ZBO_ENUM_CLASS(MyTestEnum, int, ONE, TWO, THREE)
}

struct Nester
{
    ZBO_NESTED_ENUM_CLASS(Nested, uint8_t, THREE = 3, ONE = 1, TWO, FOUR = 5);
};

TEST(EnumUtils, StringMapper)
{
    ASSERT_EQ(zbo::enumToString(MyTestEnum::ONE), "ONE");
    ASSERT_EQ(zbo::enumToString(MyTestEnum::TWO), "TWO");
    ASSERT_EQ(zbo::enumToString(MyTestEnum::THREE), "THREE");

    ASSERT_EQ(zbo::stringToEnum<MyTestEnum>("ONE"), MyTestEnum::ONE);
    ASSERT_EQ(zbo::stringToEnum<MyTestEnum>("TWO"), MyTestEnum::TWO);
    ASSERT_EQ(zbo::stringToEnum<MyTestEnum>("THREE"), MyTestEnum::THREE);
}

TEST(EnumUtils, MetaEnum)
{
    const auto& m = zbo::metaEnum<MyTestEnum>();
    const auto& m2 = zbo::metaEnum<test_namespace::MyTestEnum>();
    const auto& m3 = zbo::metaEnum<Nester::Nested>();

    ASSERT_EQ(m.size(), 3);
    ASSERT_EQ(m2.size(), 3);
    ASSERT_EQ(m3.size(), 4);

    ASSERT_EQ(m.members[1].value, TWO);
}

TEST(EnumUtils, Iteration)
{
    auto range = zbo::MetaEnumRange<MyTestEnum>();
    int enumCount = 0;
    for (auto e : range)
    {
        enumCount++;
        std::cout << zbo::enumToString<MyTestEnum>(e) << std::endl;
    }
    ASSERT_EQ(enumCount, 3);
}

TEST(EnumUtils, MetaIteratorEnum)
{
    zbo::MetaEnumIterator<MyTestEnum> it(ONE);
    it++;
    ASSERT_EQ(*it, TWO);
    it--;
    ASSERT_EQ(*it, ONE);
}

TEST(EnumUtils, MetaIteratorEnumClass)
{
    zbo::MetaEnumIterator<test_namespace::MyTestEnum> it(test_namespace::MyTestEnum::ONE);
    it++;
    ASSERT_EQ(*it, test_namespace::MyTestEnum::TWO);
    it--;
    ASSERT_EQ(*it, test_namespace::MyTestEnum::ONE);
}

TEST(EnumUtils, MetaIteration)
{
    auto range = zbo::MetaEnumRange<Nester::Nested>();
    ASSERT_EQ(range.begin()[0], Nester::Nested::THREE);
    ASSERT_EQ(range.begin()[1], Nester::Nested::ONE);
    ASSERT_EQ(range.begin()[2], Nester::Nested::TWO);
    ASSERT_EQ(range.begin()[3], Nester::Nested::FOUR);

    int enumCount = 0;
    for (auto e : range)
    {
        enumCount++;
        std::cout << static_cast<int>(e) << ":" << zbo::enumToString(e) << std::endl;
    }
    ASSERT_EQ(enumCount, 4);
}

ZBO_ENUM_CLASS(EnumWithCommnets, int,
               ONE,  //< This is the first enum in the list
               TWO,  //< This is the second enum in the list
               THREE)

TEST(EnumUtils, EnumWithComments)
{
    ASSERT_EQ(zbo::enumToString(EnumWithCommnets::ONE), "ONE");
}