#include <algorithm>
#include "mihasher.h"

using namespace std;


MIHasher::MIHasher(int bitsPerCode, int numberOfBuckets) {
    m_numberOfCodes = 0;
    m_bitsPerCode = bitsPerCode;
    B_over_8 = m_bitsPerCode / 8;

    m_numberOfBuckets = numberOfBuckets;
    m_bitsPerBucket = ceil((double) m_bitsPerCode / m_numberOfBuckets);

    m_maxHammingDistance = ceil(m_bitsPerCode / 2.0);        // assuming that B/2 is large enough radius to include all of the k nearest neighbors
    m_maxSubstringHammingDistance = ceil((double) m_maxHammingDistance / m_numberOfBuckets);

    mplus = m_bitsPerCode - m_numberOfBuckets * (m_bitsPerBucket - 1);
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

MIHasher::~MIHasher()
{
    delete[] xornum;
    delete[] H;
}


int MIHasher::getBitsPerCode() const
{
    return m_bitsPerCode;
}

int MIHasher::getNumberOfBuckets() const
{
    return m_numberOfBuckets;
}

int MIHasher::getNumberOfCodes() const {
    return m_numberOfCodes;
}

const std::vector<UINT8>& MIHasher::getCodes() const
{
    return m_vcodes;
}

/*
 * Inputs: query, numq, dim1queries
 */

/*
 * Outputs: results, numres, stats
 *
 *   results: an array of indices (1-based) of the K-nearest neighbors
 *   for each query. So the array includes K*numq uint32 integers.
 *
 *   numres: includes the number of database entries that fall at any
 *   specific Hamming distance from the query until the K nearest
 *   neighbors are reached. So from this array you can figure out the
 *   Hamming distances of the K-nearest neighbors.
 */

void MIHasher::batchquery(UINT32 *results, UINT32 *numres, qstat *stats, UINT8 *queries, UINT32 numq, int dim1queries)
{
    counter = new bitarray;
    counter->init(m_numberOfCodes);

    UINT32 *res  = new UINT32[K*(m_maxHammingDistance+1)];
    UINT64 *chunks = new UINT64[m_numberOfBuckets];

    UINT32 *presults = results;
    UINT32 *pnumres = numres;
    qstat *pstats = stats;
    UINT8 *pq = queries;

    for (int i=0; i<numq; i++) {
        query(presults, pnumres, pstats, pq, chunks, res);

        presults += K;
        pnumres += m_bitsPerCode+1;
        pstats ++;
        pq += dim1queries;
    }

    delete [] res;
    delete [] chunks;

    delete counter;
}


// Temp variables: chunks, res -- I did not want to malloc inside
// query, so these arrays are passed from outside

void MIHasher::query(UINT32 *results, UINT32* numres, qstat *stats, UINT8 *query, UINT64 *chunks, UINT32 *res)
{
    UINT32 maxres = K ? K : m_numberOfCodes;			// if K == 0 that means we want everything to be processed.
    // So maxres = N in that case. Otherwise K limits the results processed.

    UINT32 n = 0; 				// number of results so far obtained (up to a distance of s per chunk)
    UINT32 nc = 0;				// number of candidates tested with full codes (not counting duplicates)
    UINT32 nd = 0;              // counting everything retrieved (duplicates are counted multiple times)
    UINT32 nl = 0;				// number of lookups (and xors)
    UINT32 *arr;
    int size = 0;
    UINT32 index;
    int hammd;
    clock_t start, end;

    start = clock();

    counter->erase();
    memset(numres, 0, (m_bitsPerCode+1)*sizeof(*numres));

    split(chunks, query, m_numberOfBuckets, mplus, m_bitsPerBucket);


    int curb = m_bitsPerBucket;		// current b: for the first mplus substrings it is b, for the rest it is (b-1)
    int searchRadius;			// the growing search radius per substring

    for (searchRadius = 0; searchRadius <= m_maxSubstringHammingDistance && n < maxres; searchRadius++)
    {
        for (int bucketIndex = 0; bucketIndex < m_numberOfBuckets; bucketIndex++)
        {
            if (bucketIndex < mplus)
                curb = m_bitsPerBucket;
            else
                curb = m_bitsPerBucket-1;
            UINT64 chunksk = chunks[bucketIndex];
            nl += xornum[searchRadius + 1] - xornum[searchRadius];	// number of bit-strings with s number of 1s

            UINT64 bitstr = 0; 			// the bit-string with s number of 1s
            for (int i=0; i < searchRadius; i++)
                power[i] = i;			// power[i] stores the location of the i'th 1
            power[searchRadius] = curb + 1;			// used for stopping criterion (location of (s+1)th 1)

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
//				printf("%d \n", bitstr);
                    arr = H[bucketIndex].query(chunksk ^ bitstr, &size); // lookup
                    if (size) {			// the corresponding bucket is not empty
                        nd += size;
                        for (int c = 0; c < size; c++) {
                            index = arr[c];
                            if (!counter->get(index)) { // if it is not a duplicate
                                counter->set(index);
                                hammd = match(&m_vcodes[index*(B_over_8)], query, B_over_8);
                                nc++;
                                if (hammd <= m_maxHammingDistance && numres[hammd] < maxres) {
                                    res[hammd * K + numres[hammd]] = index + 1;
                                }
                                numres[hammd]++;
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
//            printf("NUmber of itertions. %d\n", numberOfIterations);
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

    end = clock();

    stats->ticks = end-start;
    stats->numcand = nc;
    stats->numdups = nd;
    stats->numlookups = nl;

    n = 0;
    for (searchRadius = 0; searchRadius <= m_maxHammingDistance && n < K; searchRadius++ ) {
        for (int c = 0; c < numres[searchRadius] && n < K; c++)
            results[n++] = res[searchRadius * K + c];
    }

    UINT32 total = 0;
    stats->maxrho = -1;
    for (int i=0; i<=m_bitsPerCode; i++) {
        total += numres[i];
        if (total >= K && stats->maxrho == -1)
            stats->maxrho = i;
    }
    stats->numres = n;
}

void MIHasher::setK(int _K)
{
    K = _K;
}

void MIHasher::insert(UINT8 *codes, UINT32 N, int dim1codes)
{
    m_numberOfCodes += N;
    m_vcodes.insert(m_vcodes.end(), &codes[0], &codes[N]);
    int k = 0;
//#pragma omp parallel shared(k)
    {
        UINT64 * chunks = new UINT64[m_numberOfBuckets];
//#pragma omp for
        for (k=0; k<m_numberOfBuckets; k++) {
            UINT8 * pcodes = codes;

            for (UINT64 i=0; i<N; i++) {
                split(chunks, pcodes, m_numberOfBuckets, mplus, m_bitsPerBucket);

                H[k].count_insert(chunks[k], i);

                if (i % (int)ceil((double)m_numberOfCodes/1000) == 0) {
                    printf("%.2f%%\r", (double)i/m_numberOfCodes * 100);
                    fflush(stdout);
                }
                pcodes += dim1codes;
            }

            // for (int k=0; k<m; k++)
            // 	H[k].allocate_mem_based_on_counts();

            pcodes = codes;
            for (UINT64 i=0; i<N; i++) {
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

}


void MIHasher::populate(UINT8 *_codes, UINT32 _N, int dim1codes)
{
    m_numberOfCodes = _N;
    m_codes = _codes;

    int k = 0;
//#pragma omp parallel shared(k)
    {
        UINT64 * chunks = new UINT64[m_numberOfBuckets];
//#pragma omp for
        for (k=0; k<m_numberOfBuckets; k++) {
            UINT8 * pcodes = m_codes;

            for (UINT64 i=0; i<m_numberOfCodes; i++) {
                split(chunks, pcodes, m_numberOfBuckets, mplus, m_bitsPerBucket);

                H[k].count_insert(chunks[k], i);

                if (i % (int)ceil((double)m_numberOfCodes/1000) == 0) {
                    printf("%.2f%%\r", (double)i/m_numberOfCodes * 100);
                    fflush(stdout);
                }
                pcodes += dim1codes;
            }

            // for (int k=0; k<m; k++)
            // 	H[k].allocate_mem_based_on_counts();

            pcodes = m_codes;
            for (UINT64 i=0; i<m_numberOfCodes; i++) {
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

    // N = _N;
    // codes = _codes;
    // UINT64 * chunks = new UINT64[m];

    // UINT8 * pcodes = codes;
    // for (UINT64 i=0; i<N; i++, pcodes += dim1codes) {
    // 	split(chunks, pcodes, m, mplus, b);

    // 	for (int k=0; k<m; k++)
    // 	    H[k].lazy_insert(chunks[k], i);
    // 	if (i % (int)ceil((double)N/1000) == 0) {
    // 	    printf("%.2f%%\r", (double)i/N * 100);
    // 	    fflush(stdout);
    // 	}
    // }

    // for (int k=0; k<m; k++)
    // 	H[k].cleanup_insert(codes, m, k, mplus, b, dim1codes);
    // delete [] chunks;
}
