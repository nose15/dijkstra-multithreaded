//
// Created by lukasz on 11.11.25.
//

#include <gtest/gtest.h>

TEST(InitialTest, BasicAssertions) {
  EXPECT_STRNE("hello", "world");
  EXPECT_EQ(7*6, 42);
}
