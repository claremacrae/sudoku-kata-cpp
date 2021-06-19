#pragma once

#include <Utilities/PortableRandomNumberGenerator.h>
namespace SudokuKata
{
    class Program
    {
    public:
        static void Play(std::ostream& console, PortableRandomNumberGenerator& rng);
    };
}
