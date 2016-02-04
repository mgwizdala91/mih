//
// Created by marcin on 29.01.16.
//

#include <iostream>
#include <cstdlib>
#include <chrono>
#include <vector>

#include "mihasher.h"

const int SEED = 260519991;
const int BITS_COUNT = 32;
const int BUCKETS_COUNT = 3;
const int QUERIES_COUNT = 400;
const int ELEMENTS_COUNT = 1000000;

int main()
{
    MIHasher miHasher(BITS_COUNT, BUCKETS_COUNT);
    miHasher.setK(10);
    srand(SEED);
//
//    for(int i = 0; i < ELEMENTS_COUNT; i=i+1000)
//    {
//        UINT32 tmp[1000];
//
//        for(int j = 0; j < 1000; j++)
//        {
//            tmp[j] = static_cast<UINT32>(rand());
//        }
//        miHasher.insert((UINT8*)tmp, 1000, 4);
//    }

    UINT8* ptr = new UINT8[ELEMENTS_COUNT*4];
    for(int i = 0; i < ELEMENTS_COUNT;i=i+4)
    {
        *((UINT32*)ptr+i) = i;
        miHasher.insert(ptr+i, 1, 4);
    }

    UINT32* queries = new UINT32[QUERIES_COUNT];
    for(int i = 0; i < QUERIES_COUNT; i++)
    {
        queries[i] = rand();
    }

    std::vector<std::vector<UINT32> > results;

    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

//    miHasher.search(ptr, ELEMENTS_COUNT, 4, results);
    miHasher.search((UINT8*)queries, QUERIES_COUNT, 4, results);

    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

    std::cout << "It took me " << time_span.count() << " seconds.";
    std::cout << std::endl;
}