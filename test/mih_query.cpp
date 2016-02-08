#include "mihasher.h"
#include "result.h"

#include <gtest/gtest.h>

TEST(QueryTest, Search) {
    UINT8 array[] = {1, 2, 7, 15, 31};

    MIHasher<8> miHasher(1);
    miHasher.setK(1);
    miHasher.insert(array, 5, 1);

    std::vector< std::vector<UINT32> > results;

    miHasher.search(array, 5, 1, results);

    ASSERT_EQ(results.size(), 5);
    ASSERT_EQ(results[0].size(), 1);
    ASSERT_EQ(results[0][0], 1);
    ASSERT_EQ(results[1].size(), 1);
    ASSERT_EQ(results[1][0], 2);
    ASSERT_EQ(results[2].size(), 1);
    ASSERT_EQ(results[2][0], 3);
    ASSERT_EQ(results[3].size(), 1);
    ASSERT_EQ(results[3][0], 4);
    ASSERT_EQ(results[4].size(), 1);
    ASSERT_EQ(results[4][0], 5);
}

TEST(QueryTest, SearchNoResult) {
    UINT8 array[] = {1, 2, 4, 8};
    UINT8 query[] = {255};

    MIHasher<8> miHasher(1);
    miHasher.setK(1);
    miHasher.insert(array, 4, 1);

    std::vector< std::vector<UINT32> > results;

    miHasher.search(query, 1, 1, results);

    ASSERT_EQ(results.size(), 1);
    ASSERT_EQ(results[0].size(), 0);
}

TEST(QueryTest, SearchEmpty) {
    UINT8 array[] = {1, 2, 7, 15, 31};

    MIHasher<8> miHasher(1);
    miHasher.setK(1);

    std::vector< std::vector<UINT32> > results;

    miHasher.search(array, 5, 1, results);

    ASSERT_EQ(results.size(), 5);
    ASSERT_EQ(results[0].size(), 0);
    ASSERT_EQ(results[1].size(), 0);
    ASSERT_EQ(results[2].size(), 0);
    ASSERT_EQ(results[3].size(), 0);
    ASSERT_EQ(results[4].size(), 0);
}