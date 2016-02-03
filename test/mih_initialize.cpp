#include "mihasher.h"
#include <gtest/gtest.h>

TEST(InitializeTest, Constructor) {
    MIHasher* miHasher = new MIHasher(8, 1);
    ASSERT_EQ(miHasher->getNumberOfCodes(), 0);
}

TEST(InitializeTest, BitCount) {
    MIHasher miHasher8(8, 1);
    MIHasher miHasher16(16, 2);
    MIHasher miHasher32(32, 4);

    ASSERT_EQ(miHasher8.getBitsPerCode(), 8);
    ASSERT_EQ(miHasher16.getBitsPerCode(), 16);
    ASSERT_EQ(miHasher32.getBitsPerCode(), 32);
}

TEST(InitializeTest, BucketCount) {
    MIHasher miHasher1(8, 1);
    MIHasher miHasher2(8, 2);
    MIHasher miHasher4(8, 4);

    ASSERT_EQ(miHasher1.getNumberOfBuckets(), 1);
    ASSERT_EQ(miHasher2.getNumberOfBuckets(), 2);
    ASSERT_EQ(miHasher4.getNumberOfBuckets(), 4);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}