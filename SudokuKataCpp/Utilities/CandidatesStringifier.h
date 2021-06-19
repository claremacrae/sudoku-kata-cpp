#pragma once

#include <string>
#include <vector>

namespace SudokuKata::Utilities
{
    class CandidatesStringifier
    {
    public:
        static std::string ConvertToString(const std::vector<int>& candidateMasks);

    private:
        static bool IsAtBlockBoundary(int value);

        static std::string
        RowForOneCell(int index, int mask, int digitIndex1, int digitIndex2, int digitIndex3);

        static std::string CharacterForDigit(int mask, int digit);
    };
}
