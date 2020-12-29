#include "stop_watch.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace zbo::test {

class MockClockImpl
{
  public:
    MOCK_METHOD(std::chrono::system_clock::time_point, now, ());
};

class MockClock
{
  public:
    // NOLINTNEXTLINE (readability-identifier-naming)
    using time_point = std::chrono::system_clock::time_point;

    static MockClockImpl& mock()
    {
        static MockClockImpl m;
        return m;
    }
    static time_point now() { return mock().now(); }
};

constexpr auto START_TIME = MockClock::time_point{};
constexpr std::chrono::duration<double> DURATION{5.0};
constexpr auto END_TIME = START_TIME + std::chrono::duration_cast<MockClock::time_point::duration>(DURATION);

TEST(StopWatch, StartStop)
{
    EXPECT_CALL(MockClock::mock(), now())
        .Times(2)
        .WillOnce(testing::Return(START_TIME))
        .WillOnce(testing::Return(END_TIME));

    StopWatchT<MockClock> watch;
    watch.start();
    ASSERT_EQ(watch.stop(), DURATION);
    // mock ensures that this will not call into MockClock::now() anymore
    ASSERT_EQ(watch.elapsed(), DURATION);

    // as the lifetime of the mock object outlives the test, we need to check the expectations explicitly
    testing::Mock::VerifyAndClearExpectations(&MockClock::mock());
}

TEST(StopWatch, StartElapsedStop)
{
    EXPECT_CALL(MockClock::mock(), now())
        .Times(3)
        .WillOnce(testing::Return(START_TIME))
        .WillOnce(testing::Return(END_TIME))
        .WillOnce(testing::Return(END_TIME));

    StopWatchT<MockClock> watch;
    watch.start();
    ASSERT_EQ(watch.elapsed(), DURATION);
    ASSERT_EQ(watch.stop(), DURATION);

    // as the lifetime of the mock object outlives the test, we need to check the expectations explicitly
    testing::Mock::VerifyAndClearExpectations(&MockClock::mock());
}

TEST(StopWatch, TimeMethod)
{
    EXPECT_CALL(MockClock::mock(), now())
        .Times(2)
        .WillOnce(testing::Return(START_TIME))
        .WillOnce(testing::Return(END_TIME));

    auto elapsed = zbo::timeFunction<MockClock>([](int i, int j) { std::cout << i << ", " << j << "\n"; }, 0, 1);
    ASSERT_EQ(elapsed, DURATION);

    // as the lifetime of the mock object outlives the test, we need to check the expectations explicitly
    testing::Mock::VerifyAndClearExpectations(&MockClock::mock());
}

}  // namespace zbo::test