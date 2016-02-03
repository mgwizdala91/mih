#ifndef BUCKET_GROUP_STL_H
#define BUCKET_GROUP_STL_H

#include <map>
#include <vector>
#include <stdio.h>
#include <math.h>
#include "types.h"
#include "array32.h"
#include "bitarray.h"

class BucketGroupSTL {

 public:

    UINT32 empty;
    std::map<int, std::vector<UINT32>*> group;

    BucketGroupSTL();

    ~BucketGroupSTL();

    void insert(int subindex, UINT32 data);

    std::vector<UINT32>* query(int subindex);

    void lazy_insert(int subindex, UINT32 data);

    void cleanup_insert(UINT8* dataset, int m, int k, int mplus, int b, int dim1codes);

    void count_insert(int subindex, UINT32 data);

    void data_insert(int subindex, UINT32 data);

    void allocate_mem_based_on_counts();

};


#endif
