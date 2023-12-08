// this code is intended to simulate the memory-cache hierachy of a computer system
// owners: Ousswa chouchane 900225937

#include <iostream>
#include <fstream>
#include <string>

#include "Cache.h"

int main()
{
    // create a file to read csv data from and save it to an array of integers
    std::ifstream file("data.csv");
    std::string line;
    int data[1000] = {0}; // to avoid garbage values
    int i = 0;
    while (std::getline(file, line, ','))
    {
        data[i] = std::stoi(line);
        i++;
    }
    // create a cache object
    Cache cache(1024, 16, 1);

    int cycles = 0;
    // read the data from the file and simulate the cache
    for (int i = 0; i < 1000; i++)
    {
        cycles += cache.directMapping(data[i]);
    }

    // print the results
    std::cout << "Direct Mapping" << std::endl;
    std::cout << "Hits: " << cache.getHits() << std::endl;
    std::cout << "Misses: " << cache.getMisses() << std::endl;
    std::cout << "Accesses: " << cache.getAccesses() << std::endl;
    std::cout << "Hit Rate: " << cache.getHitRate() << "%" << std::endl;
    std::cout << "Cycles: " << cycles << std::endl;

    return 0;
}