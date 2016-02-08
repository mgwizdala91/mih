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

TEST(HammingTest, Match8_2) {
    UINT8 p[] = {2};
    UINT8 q[] = {3};
    ASSERT_EQ(match<8>(p, q), 1);
}

TEST(HammingTest, Match8_3) {
    UINT8 p[] = {255};
    UINT8 q[] = {3};
    ASSERT_EQ(match<8>(p, q), 6);
}

TEST(HammingTest, Match8_4) {
    UINT8 p[] = {255};
    UINT8 q[] = {0};
    ASSERT_EQ(match<8>(p, q), 8);
}

TEST(HammingTest, Match8_5) {
    UINT8 p[] = {255};
    UINT8 q[] = {255};
    ASSERT_EQ(match<8>(p, q), 0);
}

TEST(HammingTest, Match8_6) {
    UINT8 p[] = {0};
    UINT8 q[] = {0};
    ASSERT_EQ(match<8>(p, q), 0);
}

TEST(HammingTest, Match16_0) {
    UINT8 p[] = {0, 0};
    UINT8 q[] = {0, 0};
    ASSERT_EQ(match<16>(p, q), 0);
}

TEST(HammingTest, Match16_1) {
    UINT8 p[] = {255, 0};
    UINT8 q[] = {0, 255};
    ASSERT_EQ(match<16>(p, q), 16);
}

TEST(HammingTest, Match16_2) {
    UINT8 p[] = {255, 255};
    UINT8 q[] = {255, 255};
    ASSERT_EQ(match<16>(p, q), 0);
}

TEST(HammingTest, Match16_3) {
    UINT8 p[] = {0, 0};
    UINT8 q[] = {0, 0};
    ASSERT_EQ(match<16>(p, q), 0);
}

TEST(HammingTest, Match16_4) {
    UINT8 p[] = {2, 3};
    UINT8 q[] = {0, 0};
    ASSERT_EQ(match<16>(p, q), 3);
}

TEST(HammingTest, Match16_5) {
    UINT8 p[] = {2, 3};
    UINT8 q[] = {4, 3};
    ASSERT_EQ(match<16>(p, q), 2);
}

TEST(HammingTest, Match32_0) {
    UINT8 p[] = {0, 0, 0, 0};
    UINT8 q[] = {0, 0, 0, 0};
    ASSERT_EQ(match<32>(p, q), 0);
}

TEST(HammingTest, Match32_1) {
    UINT8 p[] = {1, 0, 0, 0};
    UINT8 q[] = {0, 0, 0, 0};
    ASSERT_EQ(match<32>(p, q), 1);
}

TEST(HammingTest, Match32_2) {
    UINT8 p[] = {0, 1, 0, 0};
    UINT8 q[] = {0, 0, 0, 0};
    ASSERT_EQ(match<32>(p, q), 1);
}

TEST(HammingTest, Match32_3) {
    UINT8 p[] = {0, 0, 1, 0};
    UINT8 q[] = {0, 0, 0, 0};
    ASSERT_EQ(match<32>(p, q), 1);
}

TEST(HammingTest, Match32_4) {
    UINT8 p[] = {0, 0, 0, 1};
    UINT8 q[] = {0, 0, 0, 0};
    ASSERT_EQ(match<32>(p, q), 1);
}

TEST(HammingTest, Match32_5) {
    UINT8 p[] = {255, 255, 255, 255};
    UINT8 q[] = {0, 0, 0, 0};
    ASSERT_EQ(match<32>(p, q), 32);
}

TEST(HammingTest, Match32_6) {
    UINT8 p[] = {255, 255, 255, 255};
    UINT8 q[] = {0, 255, 0, 255};
    ASSERT_EQ(match<32>(p, q), 16);
}

TEST(HammingTest, Match64_0) {
    UINT8 p[] = {1, 0, 0, 0, 0, 0, 0, 0};
    UINT8 q[] = {0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_EQ(match<64>(p, q), 1);
}

TEST(HammingTest, Match64_1) {
    UINT8 p[] = {0, 1, 0, 0, 0, 0, 0, 0};
    UINT8 q[] = {0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_EQ(match<64>(p, q), 1);
}

TEST(HammingTest, Match64_2) {
    UINT8 p[] = {0, 0, 1, 0, 0, 0, 0, 0};
    UINT8 q[] = {0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_EQ(match<64>(p, q), 1);
}

TEST(HammingTest, Match64_3) {
    UINT8 p[] = {0, 0, 0, 1, 0, 0, 0, 0};
    UINT8 q[] = {0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_EQ(match<64>(p, q), 1);
}

TEST(HammingTest, Match64_4) {
    UINT8 p[] = {0, 0, 0, 0, 1, 0, 0, 0};
    UINT8 q[] = {0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_EQ(match<64>(p, q), 1);
}

TEST(HammingTest, Match64_5) {
    UINT8 p[] = {0, 0, 0, 0, 0, 1, 0, 0};
    UINT8 q[] = {0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_EQ(match<64>(p, q), 1);
}

TEST(HammingTest, Match64_6) {
    UINT8 p[] = {0, 0, 0, 0, 0, 0, 1, 0};
    UINT8 q[] = {0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_EQ(match<64>(p, q), 1);
}

TEST(HammingTest, Match64_7) {
    UINT8 p[] = {0, 0, 0, 0, 0, 0, 0, 1};
    UINT8 q[] = {0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_EQ(match<64>(p, q), 1);
}

TEST(HammingTest, Match64_8) {
    UINT8 p[] = {255, 255, 0, 0, 0, 0, 0, 0};
    UINT8 q[] = {0, 0, 255, 255, 0, 0, 0, 0};
    ASSERT_EQ(match<64>(p, q), 32);
}

TEST(HammingTest, Match64_9) {
    UINT8 p[] = {255, 255, 0, 0, 0, 0, 0, 0};
    UINT8 q[] = {0, 255, 255, 255, 0, 0, 0, 0};
    ASSERT_EQ(match<64>(p, q), 24);
}

TEST(HammingTest, Match128_0) {
    UINT8 p[] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    UINT8 q[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_EQ(match<128>(p, q), 1);
}

TEST(HammingTest, Match128_1) {
    UINT8 p[] = {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    UINT8 q[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_EQ(match<128>(p, q), 1);
}

TEST(HammingTest, Match128_2) {
    UINT8 p[] = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    UINT8 q[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_EQ(match<128>(p, q), 1);
}

TEST(HammingTest, Match128_3) {
    UINT8 p[] = {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    UINT8 q[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_EQ(match<128>(p, q), 1);
}

TEST(HammingTest, Match128_4) {
    UINT8 p[] = {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    UINT8 q[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_EQ(match<128>(p, q), 1);
}

TEST(HammingTest, Match128_5) {
    UINT8 p[] = {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    UINT8 q[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_EQ(match<128>(p, q), 1);
}

TEST(HammingTest, Match128_6) {
    UINT8 p[] = {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    UINT8 q[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_EQ(match<128>(p, q), 1);
}

TEST(HammingTest, Match128_7) {
    UINT8 p[] = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0};
    UINT8 q[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_EQ(match<128>(p, q), 1);
}

TEST(HammingTest, Match128_8) {
    UINT8 p[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0};
    UINT8 q[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_EQ(match<128>(p, q), 1);
}

TEST(HammingTest, Match128_9) {
    UINT8 p[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0};
    UINT8 q[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_EQ(match<128>(p, q), 1);
}

TEST(HammingTest, Match128_10) {
    UINT8 p[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0};
    UINT8 q[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_EQ(match<128>(p, q), 1);
}

TEST(HammingTest, Match128_11) {
    UINT8 p[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0};
    UINT8 q[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_EQ(match<128>(p, q), 1);
}

TEST(HammingTest, Match128_12) {
    UINT8 p[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0};
    UINT8 q[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_EQ(match<128>(p, q), 1);
}

TEST(HammingTest, Match128_13) {
    UINT8 p[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0};
    UINT8 q[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_EQ(match<128>(p, q), 1);
}

TEST(HammingTest, Match128_14) {
    UINT8 p[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
    UINT8 q[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_EQ(match<128>(p, q), 1);
}

TEST(HammingTest, Match128_15) {
    UINT8 p[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    UINT8 q[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_EQ(match<128>(p, q), 1);
}
