//
// Created by marcin on 07.02.16.
//
#include <bitops.h>
#include <gtest/gtest.h>

TEST(HammingTest, Match8) {
    UINT8 p[] = {1};
    UINT8 q[] = {1};
    ASSERT_EQ(match<8>(p, q), 0);
}