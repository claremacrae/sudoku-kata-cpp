#include "catch2/catch.hpp"
#include "ApprovalTests.hpp"

#include "Program.h"
#include "Utilities/PortableRandomNumberGenerator.h"

#include <iostream>
#include <iomanip>

using namespace ApprovalTests;
using namespace SudokuKata;

TEST_CASE("Should do the same thing twice")
{
    std::stringstream console;
    for (int seed = 0; seed < 10; seed++)
    {
        console << ">>>>>>>>>>>>>>>>>>>>> Output with seed: " << seed << '\n';
        PortableRandomNumberGenerator random(seed);
        Program::Play(console, random);
    }
    Approvals::verify(console.str());
}
