#include "CandidatesStringifier.h"

namespace SudokuKata::Utilities
{

    std::string CandidatesStringifier::ConvertToString(const std::vector<int>& candidateMasks)
    {
        auto blockDivider = "++=====+=====+=====++=====+=====+=====++=====+=====+=====++\n";
        auto rowDivider = "++-----+-----+-----++-----+-----+-----++-----+-----+-----++\n";
        std::string result = blockDivider;

        for (int row = 0; row < 9; row++)
        {
            std::string line1 = "||";
            std::string line2 = "||";
            std::string line3 = "||";
            for (int col = 0; col < 9; col++)
            {
                // var index = CellPosition.Index(row, col);
                auto index = (9 * row) + col;
                auto mask = candidateMasks[index];
                line1 += RowForOneCell(index, mask, 1, 2, 3);
                line2 += RowForOneCell(index, mask, 4, 5, 6);
                line3 += RowForOneCell(index, mask, 7, 8, 9);
            }

            result += line1 + "\n";
            result += line2 + "\n";
            result += line3 + "\n";
            result += IsAtBlockBoundary(row) ? blockDivider : rowDivider;
        }

        return result;
    }

    bool CandidatesStringifier::IsAtBlockBoundary(int value)
    {
        return (value + 1) % 3 == 0;
    }

    std::string CandidatesStringifier::RowForOneCell(
        int index, int mask, int digitIndex1, int digitIndex2, int digitIndex3)
    {
        std::string result = " ";
        result += CharacterForDigit(mask, digitIndex1);
        result += CharacterForDigit(mask, digitIndex2);
        result += CharacterForDigit(mask, digitIndex3);
        result += IsAtBlockBoundary(index) ? " ||" : " |";
        return result;
    }

    std::string CandidatesStringifier::CharacterForDigit(int mask, int digit)
    {
        std::string blank = ".";
        auto digitMask = 1 << (digit - 1);
        auto character = ((mask & digitMask) != 0) ? std::to_string(digit) : blank;
        return character;
    }
}
