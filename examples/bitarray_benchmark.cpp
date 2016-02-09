//
// Created by marcin on 29.01.16.
//

#include <iostream>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <bitarray.h>
#include <bitset>

const int BITS_COUNT = 1000000;

int benchmark_bitset()
{
    std::bitset<BITS_COUNT> counter;
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < 11; i++)
    {
        int value = i % 2;
        for(int j = 0; j < BITS_COUNT; j++)
        {
            counter[j] = value;
        }
    }

    int sum = 0;
    for(int i = 0; i < BITS_COUNT; i++)
    {
        sum += counter[i];
    }

    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

    std::cout << "It took me " << time_span.count() << " seconds.";
    std::cout << std::endl;
    return sum;
}

int benchmark_bitarray()
{
    bitarray counter;
    counter.init(BITS_COUNT);

    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < 11; i++)
    {
        int value = i % 2;
        for(int j = 0; j < BITS_COUNT; j++)
        {
            counter.set(j);
        }
    }

    int sum = 0;
    for(int i = 0; i < BITS_COUNT; i++)
    {
        sum += counter.get(i);
    }

    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

    std::cout << "It took me " << time_span.count() << " seconds.";
    std::cout << std::endl;
    return sum;
}

int main()
{
    benchmark_bitarray();
    benchmark_bitset();
}