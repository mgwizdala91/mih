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

template <unsigned int BITS_PER_CODE>
class MIHasher {
 private:

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

    MIHasher(int numberOfBuckets);
    ~MIHasher();

    int getBitsPerCode() const;
    int getNumberOfBuckets() const;
    int getNumberOfCodes() const;
    const std::vector<UINT8>& getCodes() const;

    void setK(int K);

    void insert(UINT8 *codes, UINT32 N, int dim1codes);

    void search(UINT8 *queries, UINT32 numq, int dim1queries, std::vector<std::vector<UINT32> >&results);

 private:
    void _search(std::vector<UINT32> &resultsVector, UINT8 *q, UINT64 * chunks);
};

#endif
