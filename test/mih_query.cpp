#include "mihasher.h"
#include "result.h"

#include <gtest/gtest.h>

TEST(QueryTest, DISABLED_Query) {
    UINT8 array[] = {1, 2, 7, 15, 31};

    MIHasher miHasher(8, 1);
    miHasher.setK(1);
    miHasher.insert(array, 5, 1);

    int NQ = 1;
    qstat *stats = (qstat*) new qstat[NQ];

    result_t result;
    result.n = 5;
    result.nq = NQ;
    result.wt = -1;
    result.cput = -1;
    result.vm = -1;
    result.rss = -1;
    result.res = NULL;
    result.nres = NULL;
    result.stats = NULL;

    result.res = (UINT32 **) malloc(sizeof(UINT32*)*NQ);
    result.res[0] = (UINT32 *) malloc(sizeof(UINT32)*1*NQ);
    for (size_t i=1; i<NQ; i++)
        result.res[i] = result.res[i-1] + 1;

    result.nres = (UINT32 **) malloc(sizeof(UINT32*)*NQ);
    result.nres[0] = (UINT32 *) malloc(sizeof(UINT32)*(8+1)*NQ);
    for (size_t i=1; i<NQ; i++)
        result.nres[i] = result.nres[i-1] + (8+1);

    result.stats = (double **) malloc(sizeof(double*)*NQ);
    result.stats[0] = (double *) malloc(sizeof(double)*STAT_DIM*NQ);
    for (size_t i=1; i<NQ; i++)
        result.stats[i] = result.stats[i-1] + STAT_DIM;

    miHasher.batchquery(result.res[0], result.nres[0], stats, &array[0], NQ, 1);

    ASSERT_EQ(*result.res[0], 1);
}


TEST(QueryTest, DISABLED_Query2) {
    UINT8 array[] = {1, 2, 7, 15, 31};

    MIHasher miHasher(8, 1);
    miHasher.setK(1);
    miHasher.insert(array, 5, 1);

    int NQ = 1;
    qstat *stats = (qstat*) new qstat[NQ];

    result_t result;
    result.n = 5;
    result.nq = NQ;
    result.wt = -1;
    result.cput = -1;
    result.vm = -1;
    result.rss = -1;
    result.res = NULL;
    result.nres = NULL;
    result.stats = NULL;

    result.res = (UINT32 **) malloc(sizeof(UINT32*)*NQ);
    result.res[0] = (UINT32 *) malloc(sizeof(UINT32)*1*NQ);
    for (size_t i=1; i<NQ; i++)
        result.res[i] = result.res[i-1] + 1;

    result.nres = (UINT32 **) malloc(sizeof(UINT32*)*NQ);
    result.nres[0] = (UINT32 *) malloc(sizeof(UINT32)*(8+1)*NQ);
    for (size_t i=1; i<NQ; i++)
        result.nres[i] = result.nres[i-1] + (8+1);

    result.stats = (double **) malloc(sizeof(double*)*NQ);
    result.stats[0] = (double *) malloc(sizeof(double)*STAT_DIM*NQ);
    for (size_t i=1; i<NQ; i++)
        result.stats[i] = result.stats[i-1] + STAT_DIM;

    miHasher.batchquery(result.res[0], result.nres[0], stats, &array[3], NQ, 1);

    ASSERT_EQ(*result.res[0], 4);
}


TEST(QueryTest, Search) {
    UINT8 array[] = {1, 2, 7, 15, 31};

    MIHasher miHasher(8, 1);
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

    MIHasher miHasher(8, 1);
    miHasher.setK(1);
    miHasher.insert(array, 4, 1);

    std::vector< std::vector<UINT32> > results;

    miHasher.search(query, 1, 1, results);

    ASSERT_EQ(results.size(), 1);
    ASSERT_EQ(results[0].size(), 0);
}

TEST(QueryTest, SearchEmpty) {
    UINT8 array[] = {1, 2, 7, 15, 31};

    MIHasher miHasher(8, 1);
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