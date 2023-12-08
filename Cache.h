#pragma once
#include <vector>
#include <cmath>

// A 24-bit memory address can be divided into three components
//|------------|--------------|-------------|
//| tag        | index        | offset      |
//|------------|--------------|-------------|

struct Block
{
    int tag;    // tag of the block
    int data;   // data stored in the block
    bool valid; // indicated if the data is valid (0 = invalid, 1 = valid)
    bool dirty; // indicates if the data has been modified (0 = not modified, 1 = modified)
};

class Cache
{
private:
    // Given to us
    int size;
    int blockSize;
    int accessCycles;

    // Calculated
    int numLines;    // Number of lines in the cache
    int tag_bits;    // Number of tag bits
    int index_bits;  // Number of index bits
    int offset_bits; // Number of offset bits
    Block *blocks;   // Array of blocks
    int hits;
    int misses;
    int accesses;

public:
    Cache(int size, int lineSize, int cyclesNeeded) : size(size), blockSize(lineSize), accessCycles(cyclesNeeded)
    {
        numLines = size / lineSize;
        // Tag bits = Total Number of bits in the address - (Number of bits for the index + Number of bits for the offset)
        tag_bits = 24 - (int)log2(numLines) - (int)log2(lineSize);
        // Index bits = log2(Number of lines in the cache)
        index_bits = (int)log2(numLines);
        // Offset bits = log2(Number of bytes in a cache line)
        offset_bits = (int)log2(lineSize);

        blocks = new Block[numLines];
        hits = 0;
        misses = 0;
        accesses = 0;
    }

    ~Cache()
    {
        delete[] blocks;
    }

    int getHits()
    {
        return hits;
    }

    int getMisses()
    {
        return misses;
    }

    int getAccesses()
    {
        return accesses;
    }

    int getHitRate()
    {
        return (hits * 100) / accesses;
    }

    int getMissRate()
    {
        return (misses * 100) / accesses;
    }

    int getAccessTime()
    {
        return (hits * accessCycles + misses * 2 * accessCycles) / accesses;
    }

    int directMapping(int address)
    {
        accesses++;
        // the tag is used to identify the memory block stored in a particular cache line
        int tag = address >> (index_bits + offset_bits);
        int mask = (0x00ffffff >> offset_bits) << offset_bits; // Removed the offset bits
        mask = (mask << tag_bits) >> tag_bits;                 // Removed the tag bits essentially leaving the index bits
        int index = (address & mask) >> offset_bits;

        if (blocks[index].valid && blocks[index].tag == tag)
        {
            hits++;
            return accessCycles;
        }
        else
        {
            misses++;
            blocks[index].valid = true;
            blocks[index].tag = tag;
            return 2 * accessCycles;
        }
    }
};