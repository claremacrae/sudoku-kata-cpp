#pragma once

#include <Utilities/PortableRandomNumberGenerator.h>
namespace SudokuKata
{
    class Program
    {
    public:
        static void Play(PortableRandomNumberGenerator& rng, std::ostream& console);
    };
}
