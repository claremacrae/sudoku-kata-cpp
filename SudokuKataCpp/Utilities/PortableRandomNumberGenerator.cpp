#include <climits>
#include "PortableRandomNumberGenerator.h"

uint32_t next_pow_2_minus_1(uint32_t v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    return v;
}

MyDistribution::MyDistribution(int smallest, int largest)
    : smallest(smallest), largest(largest)
{}

PortableRandomNumberGenerator::PortableRandomNumberGenerator() : engine(std::random_device()())
{}

PortableRandomNumberGenerator::PortableRandomNumberGenerator(int seed) : engine(seed)
{}

int PortableRandomNumberGenerator::next(int lower, int upper)
{
    MyDistribution dist(lower, upper);
    return dist(engine);
}

int PortableRandomNumberGenerator::Next()
{
    return next(0, INT_MAX);
}

int PortableRandomNumberGenerator::Next(int limit)
{
    // Returns a non-negative random integer that is less than the specified maximum.
    return next(0, limit - 1);
}
