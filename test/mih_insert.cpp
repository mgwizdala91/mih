#include "mihasher.h"
#include <gtest/gtest.h>

TEST(InsertTest, Insert) {
    UINT8 array[] = {1, 2, 7, 15, 31};

    MIHasher<8> miHasher(1);
    miHasher.insert(array, 5, 1);

    std::vector<UINT8> vec = miHasher.getCodes();

    ASSERT_EQ(miHasher.getBitsPerCode(), 8);
    ASSERT_EQ(miHasher.getNumberOfBuckets(), 1);
    ASSERT_EQ(miHasher.getNumberOfCodes(), 5);
    ASSERT_EQ(vec.size(), 5);
}


TEST(InsertTest, Insert10) {
    UINT8 array[] = {1, 2, 7, 15, 31, 64, 77, 78, 99, 121};

    MIHasher<8> miHasher(1);
    miHasher.insert(array, 10, 1);

    std::vector<UINT8> vec = miHasher.getCodes();

    ASSERT_EQ(miHasher.getBitsPerCode(), 8);
    ASSERT_EQ(miHasher.getNumberOfBuckets(), 1);
    ASSERT_EQ(miHasher.getNumberOfCodes(), 10);
    ASSERT_EQ(vec.size(), 10);
}

TEST(InsertTest, Insert100k) {
    UINT8 array[] = {1, 2, 7, 15, 31, 64, 77, 78, 99, 121};

    MIHasher<8> miHasher(1);
    for(int i = 0; i < 10000; i++)
    {
        miHasher.insert(array, 10, 1);
    }

    std::vector<UINT8> vec = miHasher.getCodes();

    ASSERT_EQ(miHasher.getBitsPerCode(), 8);
    ASSERT_EQ(miHasher.getNumberOfBuckets(), 1);
    ASSERT_EQ(miHasher.getNumberOfCodes(), 100000);
    ASSERT_EQ(vec.size(), 100000);
}

TEST(InsertTest, InsertTwice) {
    UINT8 array[] = {1, 2, 7, 15, 31};
    UINT8 array2[] = {64, 77, 78, 99, 121};

    MIHasher<8> miHasher(1);
    miHasher.insert(array, 5, 1);

    std::vector<UINT8> vec = miHasher.getCodes();

    ASSERT_EQ(miHasher.getNumberOfCodes(), 5);
    ASSERT_EQ(vec.size(), 5);

    vec = miHasher.getCodes();
    for (int i = 0; i < 5; i++) {
        ASSERT_EQ(vec[i], array[i]);
    }

    miHasher.insert(array2, 5, 1);

    vec = miHasher.getCodes();
    ASSERT_EQ(miHasher.getNumberOfCodes(), 10);

    for (int i = 0; i < 5; i++) {
        ASSERT_EQ(vec[i], array[i]);
    }
    for (int i = 5; i < 10; i++) {
        ASSERT_EQ(vec[i], array2[i-5]);
    }

    ASSERT_EQ(vec.size(), 10);
}


TEST(InsertTest, Insert_16bit) {
    UINT16 array[] = {1, 2, 7, 15, 31};
    UINT8* arrayPointer = (UINT8*)array;

    MIHasher<16> miHasher(1);
    miHasher.insert(arrayPointer, 5, 2);

    std::vector<UINT8> vec = miHasher.getCodes();

    ASSERT_EQ(miHasher.getBitsPerCode(), 16);
    ASSERT_EQ(miHasher.getNumberOfBuckets(), 1);
    ASSERT_EQ(miHasher.getNumberOfCodes(), 5);
    ASSERT_EQ(vec.size(), 10);
}


TEST(InsertTest, Insert_32bit) {
    UINT32 array[] = {1, 2, 7, 15, 31};
    UINT8* arrayPointer = (UINT8*)array;

    MIHasher<32> miHasher(2);
    miHasher.insert(arrayPointer, 5, 2);

    std::vector<UINT8> vec = miHasher.getCodes();

    ASSERT_EQ(miHasher.getBitsPerCode(), 32);
    ASSERT_EQ(miHasher.getNumberOfBuckets(), 2);
    ASSERT_EQ(miHasher.getNumberOfCodes(), 5);
    ASSERT_EQ(vec.size(), 20);
}