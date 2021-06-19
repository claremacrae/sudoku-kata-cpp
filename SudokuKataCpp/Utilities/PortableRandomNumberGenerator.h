#pragma once

#include <random>

// Taken from code by Ben Craig at https://godbolt.org/z/dorhqr9EM

uint32_t next_pow_2_minus_1(uint32_t v);

struct MyDistribution
{
    int smallest;
    int largest;
    MyDistribution(int smallest, int largest);

    template<typename Engine>
    int operator()(Engine &e)
    {
        uint32_t range = largest - smallest;
        if ( range == 0 )
        {
            return smallest;
        }
        uint32_t mask = next_pow_2_minus_1(range);
        uint32_t candidate = e() & mask;
        while (candidate > range) {
            candidate = e() & mask;
        }
        return int(candidate) + smallest;
    }
};

class PortableRandomNumberGenerator
{
private:
    std::mt19937 engine;

public:
    PortableRandomNumberGenerator();
    explicit PortableRandomNumberGenerator(int seed);
    [[nodiscard]] int next(int lower, int upper);

    int Next();
    int Next(int limit);
};
