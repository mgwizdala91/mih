#include "sparse_hashtable.h"
#include "bucket_group_stl.h"
#include <iostream>

const int SparseHashtable::MAX_B = 37;

SparseHashtable::SparseHashtable() {
    table = nullptr;
    size = 0;
    b = 0;
}

int SparseHashtable::init(int _b) {
    b = _b;
    if (b < 5 || b > MAX_B || b > sizeof(UINT64)*8)
	return 1;
    
    size = UINT64_1 << (b-5);	// size = 2 ^ b
//    table = (BucketGroup*) calloc((size_t)size, sizeof(BucketGroup));
    table = new BucketGroupSTL[size];
    return 0;
}

SparseHashtable::~SparseHashtable () {
    delete[] table;
}

void SparseHashtable::insert(UINT64 index, UINT32 data) {
    /* index & 31 is equivalent to % 32 */
    table[index >> 5].insert((int)(index & 31), data);
}

std::vector<UINT32>* SparseHashtable::query(UINT64 index) {
    /* index & 31 is equivalent to % 32 */
    return table[index >> 5].query((int)(index & 31));
}


void SparseHashtable::lazy_insert(UINT64 index, UINT32 data) {
    /* index & 31 is equivalent to % 32 */
    table[index >> 5].lazy_insert((int)(index & 31), data);
}

void SparseHashtable::cleanup_insert(UINT8* dataset, int m, int k, int mplus, int b, int dim1codes) {
//    for (UINT64 i=0; i<size; i++)
//	if (table[i].group != NULL)
//	    table[i].cleanup_insert(dataset, m, k, mplus, b, dim1codes);
}


void SparseHashtable::count_insert(UINT64 index, UINT32 data) {
    /* index & 31 is equivalent to % 32 */
    table[index >> 5].count_insert((int)(index & 31), data);
}

void SparseHashtable::allocate_mem_based_on_counts() {
//    for (UINT64 i=0; i<size; i++)
//	if (table[i].group != NULL)
//	    table[i].allocate_mem_based_on_counts();
}

void SparseHashtable::data_insert(UINT64 index, UINT32 data) {
    /* index & 31 is equivalent to % 32 */
    try
    {
        table[index >> 5].data_insert((int)(index & 31), data);
    }
    catch(int e)
    {
        std::cout << "Accessed index: " << (index >> 5) <<std::endl;
    }
}
