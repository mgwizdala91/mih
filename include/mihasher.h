#ifndef __MIHASHER_H
#define __MIHASHER_H

#include <vector>

#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "types.h"
#include "bitops.h"

#include "sparse_hashtable.h"
#include "bitarray.h"

#define STAT_DIM 6		/* Dimensionality of stats, it has STAT_DIM many fields */

struct qstat {
    UINT32 numres;		// Total number of returned results
    UINT32 numcand;		// Number of hamming distance computations executed
    UINT32 numdups;		// Number of candidates skipped because they were duplicates
    UINT32 numlookups;
    UINT32 maxrho;		// Largest distance that was searched exhaustively
    clock_t ticks;		// Number of clock ticks spent on each query
};

class MIHasher {
 private:

    int m_bitsPerCode;			// Bits per code

    int B_over_8;

    int m_bitsPerBucket;			// Bits per chunk (must be less than 64)

    int m_numberOfBuckets;			// Number of chunks

    int mplus;			// Number of chunks with b bits (have 1 bit more than others)

    int m_maxHammingDistance;			// Maximum hamming search radius (we use B/2 by default)

    int m_maxSubstringHammingDistance;			// Maximum hamming search radius per substring

    int K;			// TODO move this as query function param

    UINT64 m_numberOfCodes;			// Number of codes

    std::vector<UINT8> m_vcodes;
    UINT8* m_codes;		// Table of original full-length codes

    /* is not thread safe */
    bitarray *counter;		// Counter for eliminating duplicate results
	
    SparseHashtable *H;		// Array of m hashtables;
		
    UINT32 *xornum;		// Volume of a b-bit Hamming ball with radius s (for s = 0 to d)

    int power[100];		// Used within generation of binary codes at a certain Hamming distance

 public:

    MIHasher(int bitsPerCode, int numberOfBuckets);
    ~MIHasher();

    int getBitsPerCode() const;
    int getNumberOfBuckets() const;
    int getNumberOfCodes() const;
    const std::vector<UINT8>& getCodes() const;

    void setK(int K);

    void insert(UINT8 *codes, UINT32 N, int dim1codes);
    void populate(UINT8 *codes, UINT32 N, int dim1codes);

    void search(UINT8 *queries, UINT32 numq, int dim1queries, std::vector<std::vector<UINT32> >&results);


    void batchquery (UINT32 *results, UINT32 *numres, qstat *stats, UINT8 * q, UINT32 numq, int dim1queries);
   	
 private:
    void query(UINT32 *results, UINT32* numres, qstat *stats, UINT8 *q, UINT64 * chunks, UINT32 * res);

    void _search(std::vector<UINT32> &resultsVector, UINT8 *q, UINT64 * chunks);
};

#endif
