#ifndef BITOPTS_H__
#define BITOPTS_H__

#include <stdio.h>
#include <math.h>
#include "types.h"


//#if defined(USE_SSE) && defined(__SSSE3__)
//#include <tmmintrin.h>
//
//__m128i popcount_ssse3_helper_1(unsigned* buf, int N) {
//    __m128i* vbuf = (__m128i*)buf;
//    __m128i total = _mm_setzero_si128();
//    // LUT of count of set bits in each possible 4-bit nibble, from low-to-high:
//    // 0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4
//    const unsigned _LUT[] = {0x02010100, 0x03020201, 0x03020201, 0x04030302};
//    const __m128i LUT = _mm_load_si128((__m128i*)_LUT);
//    const __m128i mask = _mm_set1_epi32(0x0F0F0F0F);
//
//    for (int i = 0; i < N; i+=4) {
//        __m128i v0 = _mm_load_si128(vbuf+i+0);
//        __m128i v1 = _mm_load_si128(vbuf+i+1);
//        __m128i v2 = _mm_load_si128(vbuf+i+2);
//        __m128i v3 = _mm_load_si128(vbuf+i+3);
//
//        // Split each byte into low and high nibbles
//        __m128i v0_lo = _mm_and_si128(mask,v0);
//        __m128i v1_lo = _mm_and_si128(mask,v1);
//        __m128i v2_lo = _mm_and_si128(mask,v2);
//        __m128i v3_lo = _mm_and_si128(mask,v3);
//
//        __m128i v0_hi = _mm_and_si128(mask,_mm_srli_epi16(v0,4));
//        __m128i v1_hi = _mm_and_si128(mask,_mm_srli_epi16(v1,4));
//        __m128i v2_hi = _mm_and_si128(mask,_mm_srli_epi16(v2,4));
//        __m128i v3_hi = _mm_and_si128(mask,_mm_srli_epi16(v3,4));
//
//        // Compute POPCNT of each byte in two halves using PSHUFB instruction for LUT
//        __m128i count0 = _mm_add_epi8(_mm_shuffle_epi8(LUT,v0_lo),_mm_shuffle_epi8(LUT,v0_hi));
//        __m128i count1 = _mm_add_epi8(_mm_shuffle_epi8(LUT,v1_lo),_mm_shuffle_epi8(LUT,v1_hi));
//        __m128i count2 = _mm_add_epi8(_mm_shuffle_epi8(LUT,v2_lo),_mm_shuffle_epi8(LUT,v2_hi));
//        __m128i count3 = _mm_add_epi8(_mm_shuffle_epi8(LUT,v3_lo),_mm_shuffle_epi8(LUT,v3_hi));
//
//        total = _mm_add_epi8(total,_mm_add_epi8(_mm_add_epi8(count0,count1),
//                                                _mm_add_epi8(count2,count3)));
//
//    }
//    // Reduce 16*8b->{-,-,-,16b,-,-,-,16b}
//    const __m128i ZERO = _mm_setzero_si128();
//    return _mm_sad_epu8(total,ZERO);
//}
//
//
//inline int popcount_ssse3(unsigned* buf,int n) {
//    int N = n/4;
//    __m128i count32 = _mm_setzero_si128();
//    // 2^5 loop iters might overflow 8-bit counter, so
//    // cap it at 2^4 iters per chunk
//    const int inner_maxits = 16;
//    while (N > inner_maxits) {
//        count32 = _mm_add_epi32(count32,popcount_ssse3_helper_1(buf,inner_maxits));
//        buf += inner_maxits*4;
//        N -= inner_maxits;
//    }
//    if (N > 0) count32 = _mm_add_epi32(count32,popcount_ssse3_helper_1(buf,N));
//
//    // Layout coming from PSADBW accumulation is 2*{0,32}: 0 S1 0 S0
//    int count;
//    _mm_store_ss((float*)&count,(__m128)(_mm_add_epi32(count32,_mm_shuffle_epi32(count32,_MM_SHUFFLE(2,2,2,2)))));
//    return count;
//}
//
//template <unsigned int B>
//inline int match(UINT8* P, UINT8* Q)
//{
//	int output = 0;
//	for (int i=0; i<B; i++)
//		output+= lookup[P[i] ^ Q[i]];
//	return output;
//}
//
//template <>
//inline int match<32>(UINT8* P, UINT8* Q)
//{
//	return popcount_ssse3((UINT32*)P[0] ^ (UINT32*)Q[0], );
//}
//
//#else

#define popcntll __builtin_popcountll
#define popcnt __builtin_popcount

const int lookup [] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8};

template <unsigned int B>
inline int match(UINT8* P, UINT8* Q)
{
	int output = 0;
	for (int i=0; i<B/8; i++)
		output+= lookup[P[i] ^ Q[i]];
	return output;
}


extern uint8_t POPCOUNT_4bit[16];

template <>
inline int match<32>(UINT8* P, UINT8* Q)
{
	return popcnt(*(UINT32*)P ^ *(UINT32*)Q);
}

template <>
inline int match<64>(UINT8* P, UINT8* Q)
{
	return popcntll(((UINT64*)P)[0] ^ ((UINT64*)Q)[0]);
}

template <>
inline int match<128>(UINT8* P, UINT8* Q)
{
	return popcntll(((UINT64*)P)[0] ^ ((UINT64*)Q)[0]) \
	    + popcntll(((UINT64*)P)[1] ^ ((UINT64*)Q)[1]);
}

template <>
inline int match<256>(UINT8* P, UINT8* Q)
{
	return popcntll(((UINT64*)P)[0] ^ ((UINT64*)Q)[0]) \
	    + popcntll(((UINT64*)P)[1] ^ ((UINT64*)Q)[1]) \
	    + popcntll(((UINT64*)P)[2] ^ ((UINT64*)Q)[2]) \
	    + popcntll(((UINT64*)P)[3] ^ ((UINT64*)Q)[3]);
}

template <>
inline int match<512>(UINT8* P, UINT8* Q)
{
	return popcntll(((UINT64*)P)[0] ^ ((UINT64*)Q)[0]) \
	    + popcntll(((UINT64*)P)[1] ^ ((UINT64*)Q)[1]) \
	    + popcntll(((UINT64*)P)[2] ^ ((UINT64*)Q)[2]) \
	    + popcntll(((UINT64*)P)[3] ^ ((UINT64*)Q)[3]) \
	    + popcntll(((UINT64*)P)[4] ^ ((UINT64*)Q)[4]) \
	    + popcntll(((UINT64*)P)[5] ^ ((UINT64*)Q)[5]) \
	    + popcntll(((UINT64*)P)[6] ^ ((UINT64*)Q)[6]) \
	    + popcntll(((UINT64*)P)[7] ^ ((UINT64*)Q)[7]);
}

/* b <= 64 */
inline void split (UINT64 *chunks, UINT8 *code, int m, int mplus, int b) {
	UINT64 temp = 0x0;
	int nbits = 0;
	int nbyte = 0;
	UINT64 mask = b==64 ? 0xFFFFFFFFFFFFFFFFLLU : ((UINT64_1 << b) - UINT64_1);

	for (int i=0; i<m; i++) {
		while (nbits < b) {
			temp |= ((UINT64)code[nbyte++] << nbits);
			nbits += 8;
		}
		chunks[i] = temp & mask;
		temp = b==64 ? 0x0 : temp >> b;
		nbits -= b;
		if (i == mplus-1) {
			b--;		/* b <= 63 */
			mask = ((UINT64_1 << b) - UINT64_1);
		}
	}
}

/* generates the next binary code (in alphabetical order) with the
 * same number of ones as the input x. Taken from
 * http://www.geeksforgeeks.org/archives/10375
 */
inline UINT64 next_set_of_n_elements(UINT64 x) {
	UINT64 smallest, ripple, new_smallest;

	smallest     = x & -x;
	ripple       = x + smallest;
	new_smallest = x ^ ripple;
	new_smallest = new_smallest / smallest;
	new_smallest >>= 2;
	return ripple | new_smallest;
}

inline void print_code(UINT64 tmp, int b) {
	for (int j=(b-1); j>=0; j--) {
		printf("%llu", (long long int) tmp/(1 << j));
		tmp = tmp - (tmp/(1 << j)) * (1 << j);
	}
	printf("\n");
}

inline UINT64 choose(int n, int r) {
	UINT64 nchooser = 1;
	for (int k=0; k < r; k++) {
		nchooser *= n-k;
		nchooser /= k+1;
	}
	return nchooser;
}

#endif
//
//#endif
