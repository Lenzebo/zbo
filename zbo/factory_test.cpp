#include "factory.h"

#include <gtest/gtest.h>

namespace zbo::test {

struct TestInterface
{
    using Key = std::string;
    virtual ~TestInterface() = default;
};

struct TestInstance1 : public TestInterface
{
    static constexpr auto ID = "test1";
};

struct TestInstance2 : public TestInterface
{
    static constexpr auto ID = "test2";
};

ZBO_REGISTER_IN_FACTORY(TestInstance1, TestInterface, TestInstance1::ID);
ZBO_REGISTER_IN_FACTORY(TestInstance2, TestInterface, TestInstance2::ID);

using TestFactory = Factory<TestInterface>;

TEST(Factory, ClassesAreRegistered)
{
    std::unique_ptr<TestInterface> in1 = TestFactory::make(TestInstance1::ID);
    std::unique_ptr<TestInterface> in2 = TestFactory::make(TestInstance2::ID);

    ASSERT_TRUE(in1);
    ASSERT_TRUE(in2);

    // check that we can cast it to the specific function
    ASSERT_TRUE(dynamic_cast<TestInstance1*>(in1.get()));
    ASSERT_TRUE(dynamic_cast<TestInstance2*>(in2.get()));
}

TEST(Factory, InvalidKey)
{
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-goto)
    ASSERT_THROW(TestFactory::make("non-existing"), std::invalid_argument);
}

TEST(Factory, AvailableKeys)
{
    const auto keys = TestFactory::getAvailableKeys();
    const std::vector<std::string> expectedKeys{TestInstance2::ID, TestInstance1::ID};
    ASSERT_TRUE(std::is_permutation(keys.begin(), keys.end(), expectedKeys.begin()));
}

class DummyInterface
{
};

class DummyImpl : public DummyInterface
{
};

TEST(Factory, InterfaceWithoutKeyType)
{
    using DummyFactory = Factory<DummyInterface, int>;
    DummyFactory::registerType<DummyImpl>(0);
    auto dummyInstance = DummyFactory::make(0);
    ASSERT_TRUE(dummyInstance);
}

}  // namespace zbo::test