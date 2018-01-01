//
//  BloomFilter.h
//  BloomFilter
//
//  Created by Claus Guttesen on 14.07.2017.
//  Copyright © 2017 Claus Guttesen. All rights reserved.
//

#ifndef BloomFilter_h
#define BloomFilter_h

#include <vector>
#include <array>

#include "MurmurHash3.hpp"

struct BloomFilter {
    BloomFilter(uint64_t size, uint8_t numHashes);
    
    void add(const uint8_t *data, std::size_t len);
    bool possiblyContains(const uint8_t *data, std::size_t len) const;
    
private:
    uint8_t m_numHashes;
    std::vector<bool> m_bits;
};

#endif /* BloomFilter_h */