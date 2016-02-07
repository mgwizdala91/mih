#include "mihasher.h"
#include <gtest/gtest.h>

TEST(InitializeTest, Constructor) {
    MIHasher<8>* miHasher = new MIHasher<8>(1);
    ASSERT_EQ(miHasher->getNumberOfCodes(), 0);
}

TEST(InitializeTest, BitCount) {
    MIHasher<8> miHasher8(1);
    MIHasher<16> miHasher16(2);
    MIHasher<32> miHasher32(4);

    ASSERT_EQ(miHasher8.getBitsPerCode(), 8);
    ASSERT_EQ(miHasher16.getBitsPerCode(), 16);
    ASSERT_EQ(miHasher32.getBitsPerCode(), 32);
}

TEST(InitializeTest, BucketCount) {
    MIHasher<8> miHasher1(1);
    MIHasher<8> miHasher2(2);
    MIHasher<8> miHasher4(4);

    ASSERT_EQ(miHasher1.getNumberOfBuckets(), 1);
    ASSERT_EQ(miHasher2.getNumberOfBuckets(), 2);
    ASSERT_EQ(miHasher4.getNumberOfBuckets(), 4);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}