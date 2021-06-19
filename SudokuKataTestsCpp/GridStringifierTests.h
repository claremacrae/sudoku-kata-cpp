#pragma once

#include <vector>

namespace SudokuKataTests
{
    class GridStringifierTests
    {
    public:
        static void VerifyBoardAsString(std::vector<int>& input);

        static void VerifyBoardAsCode(std::vector<int>& input);
    };
}
