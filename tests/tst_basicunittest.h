#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>



TEST(BasicTestSet, BasicTest)
{
    EXPECT_EQ(1, 1);
    ASSERT_THAT(0, testing::Eq(0));
}
