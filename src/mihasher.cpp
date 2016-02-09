#include <algorithm>
#include "mihasher.h"

#include <iostream>
#include <bitset>

using namespace std;

uint8_t POPCOUNT_4bit[16] = {
        /* 0 */ 0,
        /* 1 */ 1,
        /* 2 */ 1,
        /* 3 */ 2,
        /* 4 */ 1,
        /* 5 */ 2,
        /* 6 */ 2,
        /* 7 */ 3,
        /* 8 */ 1,
        /* 9 */ 2,
        /* a */ 2,
        /* b */ 3,
        /* c */ 2,
        /* d */ 3,
        /* e */ 3,
        /* f */ 4
};

template <unsigned int BITS_PER_CODE>
MIHasher<BITS_PER_CODE>::MIHasher(int numberOfBuckets) {
    m_numberOfCodes = 0;
    B_over_8 = BITS_PER_CODE / 8;

    m_numberOfBuckets = numberOfBuckets;
    m_bitsPerBucket = ceil((double) BITS_PER_CODE / m_numberOfBuckets);

    m_maxHammingDistance = ceil(BITS_PER_CODE / 2.0);        // assuming that B/2 is large enough radius to include all of the k nearest neighbors
    m_maxSubstringHammingDistance = ceil((double) m_maxHammingDistance / m_numberOfBuckets);

    mplus = BITS_PER_CODE - m_numberOfBuckets * (m_bitsPerBucket - 1);
    // mplus     is the number of chunks with b bits
    // (m-mplus) is the number of chunks with (b-1) bits

    xornum = new UINT32[m_maxSubstringHammingDistance + 2];
    xornum[0] = 0;
    for (int i = 0; i <= m_maxSubstringHammingDistance; i++) {
        xornum[i + 1] = xornum[i] + choose(m_bitsPerBucket, i);
    }

    H = new SparseHashtable[m_numberOfBuckets];
    // H[i].init might fail
    for (int i = 0; i < mplus; i++) {
        H[i].init(m_bitsPerBucket);
    }

    for (int i = mplus; i < m_numberOfBuckets; i++)
    {
        H[i].init(m_bitsPerBucket-1);
    }
}

template <unsigned int BITS_PER_CODE>
MIHasher<BITS_PER_CODE>::~MIHasher()
{
    delete[] xornum;
    delete[] H;
}

template <unsigned int BITS_PER_CODE>
int MIHasher<BITS_PER_CODE>::getBitsPerCode() const
{
    return BITS_PER_CODE;
}

template <unsigned int BITS_PER_CODE>
int MIHasher<BITS_PER_CODE>::getNumberOfBuckets() const
{
    return m_numberOfBuckets;
}

template <unsigned int BITS_PER_CODE>
int MIHasher<BITS_PER_CODE>::getNumberOfCodes() const {
    return m_numberOfCodes;
}

template <unsigned int BITS_PER_CODE>
const std::vector<UINT8>& MIHasher<BITS_PER_CODE>::getCodes() const
{
    return m_vcodes;
}

template <unsigned int BITS_PER_CODE>
void MIHasher<BITS_PER_CODE>::search(UINT8 *queries, UINT32 numq, int dim1queries, std::vector<std::vector<UINT32> > &results) {

    //TODO shared counter is dangerous. Lib cannot be used by multiple threads at once!
    counter = new bitarray;
    counter->init(m_numberOfCodes);

    //allocate this region of memory once for all queries. This probably doesent give much performance. Consider moving it to _search function to simplify functions arguments
    UINT64 *chunks = new UINT64[m_numberOfBuckets];

    UINT8 *pq = queries;

    for (int i=0; i<numq; i++) {
        std::vector<UINT32> singleQueryResults;

        _search(singleQueryResults, pq, chunks);
        pq += dim1queries;
        results.push_back(singleQueryResults);
    }

    delete [] chunks;
    delete counter;
}

// Temp variables: chunks, res -- I did not want to malloc inside
// query, so these arrays are passed from outside

template <unsigned int BITS_PER_CODE>
void MIHasher<BITS_PER_CODE>::_search(std::vector<UINT32> &resultsVector, UINT8 *query, UINT64 *chunks) {

    UINT32 maxres = K ? K : m_numberOfCodes;			// if K == 0 that means we want everything to be processed.
    // So maxres = N in that case. Otherwise K limits the results processed.

    UINT32 n = 0; 				// number of results so far obtained (up to a distance of s per chunk)
    UINT32 nc = 0;				// number of candidates tested with full codes (not counting duplicates)
    UINT32 nd = 0;              // counting everything retrieved (duplicates are counted multiple times)
    UINT32 numberOfPerformedLookups = 0;				// number of lookups (and xors)
    std::vector<UINT32> *arr;
    int size = 0;
    UINT32 index;
    int hammd;
    clock_t start, end;

//    start = clock();

    counter->erase();


    UINT32 res[K*(m_maxHammingDistance+1)];

    UINT32* numres = new UINT32[BITS_PER_CODE+1];

    memset(numres, 0, (BITS_PER_CODE+1)*sizeof(*numres));

    split(chunks, query, m_numberOfBuckets, mplus, m_bitsPerBucket);


    int currentBitsCount = m_bitsPerBucket;		// current b: for the first mplus substrings it is b, for the rest it is (b-1)
    int searchRadius;			// the growing search radius per substring

    for (searchRadius = 0; searchRadius <= m_maxSubstringHammingDistance && n < maxres; searchRadius++)
    {
        for (int bucketIndex = 0; bucketIndex < m_numberOfBuckets; bucketIndex++)
        {
            if (bucketIndex < mplus)
                currentBitsCount = m_bitsPerBucket;
            else
                currentBitsCount = m_bitsPerBucket-1;

            UINT64 chunksk = chunks[bucketIndex];
            numberOfPerformedLookups += xornum[searchRadius + 1] - xornum[searchRadius];	// number of bit-strings with s number of 1s

            UINT64 bitstr = 0; 			// the bit-string with s number of 1s
            for (int i=0; i < searchRadius; i++)
                power[i] = i;			// power[i] stores the location of the i'th 1
            power[searchRadius] = currentBitsCount + 1;			// used for stopping criterion (location of (s+1)th 1)

            int bit = searchRadius - 1;			// bit determines the 1 that should be moving to the left
            // we start from the left-most 1, and move it to the left until it touches another one


            int numberOfIterations = 0;

            while (true) {			// the loop for changing bitstr
                if (bit != -1) {
                    bitstr ^= (power[bit] == bit) ? (UINT64)1 << power[bit] : (UINT64)3 << (power[bit]-1);
                    power[bit]++;
                    bit--;
                    numberOfIterations++;
                } else { // bit == -1
                    /* the binary code bitstr is available for processing */

//                    std::cout << "index = " << bucketIndex << " bitstr = " << std::bitset<8>(bitstr)  << std::endl;
                    arr = H[bucketIndex].query(chunksk ^ bitstr); // lookup
                    if(arr)
                    {
//                    if (size) {			// the corresponding bucket is not empty
//                        nd += size;
                        const int vectorSize = arr->size();
                        for (int c = 0; c < vectorSize; c++) {
                            index = (*arr)[c];

                            if (!counter->get(index)) { // if it is not a duplicate
                                counter->set(index);
                                hammd = match<BITS_PER_CODE>(&m_vcodes[index*(B_over_8)], query);
                                nc++;
                                if (hammd <= m_maxHammingDistance && numres[hammd] < maxres) {
                                    res[hammd * K + numres[hammd]] = index + 1;
                                    ++numres[hammd];
                                }
                            }
                        }
                    }
                    /* end of processing */

                    while (++bit < searchRadius && power[bit] == power[bit + 1] - 1) {
                        bitstr ^= (UINT64)1 << (power[bit]-1);
                        power[bit] = bit;
                    }
                    if (bit == searchRadius)
                        break;
                }
            }

            n = n + numres[searchRadius * m_numberOfBuckets + bucketIndex]; // This line is very tricky ;)
            // The k'th substring (0 based) is the last chance of an
            // item at a Hamming distance of s*m+k to be
            // found. Because if until the k'th substring, an item
            // with distance of s*m+k is not found, then it means that
            // all of the substrings so far have a distance of (s+1)
            // or more, and the remaining substrings have a distance
            // of s or more (total > s*m+k).

            if (n >= maxres)
                break;
        }
    }

//    end = clock();

//    stats->ticks = end-start;
//    stats->numcand = nc;
//    stats->numdups = nd;
//    stats->numlookups = nl;
//
    n = 0;
    for (searchRadius = 0; searchRadius <= m_maxHammingDistance && n < K; searchRadius++ ) {
        for (int c = 0; c < numres[searchRadius] && n < K; c++)
        {
            resultsVector.push_back(res[searchRadius * K + c]); n++;
        }
    }

    delete[] numres;

//    UINT32 total = 0;
//    stats->maxrho = -1;
//    for (int i=0; i<=m_bitsPerCode; i++) {
//        total += numres[i];
//        if (total >= K && stats->maxrho == -1)
//            stats->maxrho = i;
//    }
//    stats->numres = n;
}

template <unsigned int BITS_PER_CODE>
void MIHasher<BITS_PER_CODE>::setK(int _K)
{
    K = _K;
}

template <unsigned int BITS_PER_CODE>
void MIHasher<BITS_PER_CODE>::insert(UINT8 *codes, UINT32 N, int dim1codes)
{
    m_numberOfCodes += N;
    m_vcodes.insert(m_vcodes.end(), &codes[0], &codes[N*(BITS_PER_CODE/8)]);
    int k = 0;
    UINT64 * chunks = new UINT64[m_numberOfBuckets];
    for (k=0; k<m_numberOfBuckets; k++) {
        UINT8 * pcodes = codes;

        for (UINT64 i=m_numberOfCodes-N; i<m_numberOfCodes; i++) {
            split(chunks, pcodes, m_numberOfBuckets, mplus, m_bitsPerBucket);
            H[k].count_insert(chunks[k], i);
            pcodes += dim1codes;
        }

        pcodes = codes;
        for (UINT64 i=m_numberOfCodes-N; i<m_numberOfCodes; i++) {
            split(chunks, pcodes, m_numberOfBuckets, mplus, m_bitsPerBucket);
            H[k].data_insert(chunks[k], i);

            if (i % (int)ceil((double)m_numberOfCodes/1000) == 0) {
                printf("%.2f%%\r", (double)i/m_numberOfCodes * 100);
                fflush(stdout);
            }
            pcodes += dim1codes;
        }
    }

    delete [] chunks;
}

//template <unsigned int BITS_PER_CODE>
//void MIHasher<BITS_PER_CODE>::populate(UINT8 *_codes, UINT32 _N, int dim1codes)
//{
//    m_numberOfCodes = _N;
//    m_codes = _codes;
//
//    int k = 0;
////#pragma omp parallel shared(k)
//    {
//        UINT64 * chunks = new UINT64[m_numberOfBuckets];
////#pragma omp for
//        for (k=0; k<m_numberOfBuckets; k++) {
//            UINT8 * pcodes = m_codes;
//
//            for (UINT64 i=0; i<m_numberOfCodes; i++) {
//                split(chunks, pcodes, m_numberOfBuckets, mplus, m_bitsPerBucket);
//
//                H[k].count_insert(chunks[k], i);
//
//                if (i % (int)ceil((double)m_numberOfCodes/1000) == 0) {
//                    printf("%.2f%%\r", (double)i/m_numberOfCodes * 100);
//                    fflush(stdout);
//                }
//                pcodes += dim1codes;
//            }
//
//            // for (int k=0; k<m; k++)
//            // 	H[k].allocate_mem_based_on_counts();
//
//            pcodes = m_codes;
//            for (UINT64 i=0; i<m_numberOfCodes; i++) {
//                split(chunks, pcodes, m_numberOfBuckets, mplus, m_bitsPerBucket);
//
//                H[k].data_insert(chunks[k], i);
//
//                if (i % (int)ceil((double)m_numberOfCodes/1000) == 0) {
//                    printf("%.2f%%\r", (double)i/m_numberOfCodes * 100);
//                    fflush(stdout);
//                }
//                pcodes += dim1codes;
//            }
//        }
//
//        delete [] chunks;
//    }
//}

template class MIHasher<128>;
template class MIHasher<64>;
template class MIHasher<32>;
template class MIHasher<16>;
template class MIHasher<8>;