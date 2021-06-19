#pragma once

#include <string>
#include <vector>

namespace SudokuKata::Utilities
{
    class GridStringifier
    {
    public:
        static std::string ConvertToString(const std::vector<int>& board);

    private:
        static std::string ToRowNumbered(const std::vector<int>& board, int row);

        static std::string ToRow(const std::vector<int>& board,
                                 int index0,
                                 int index1,
                                 int index2,
                                 int index3,
                                 int index4,
                                 int index5,
                                 int index6,
                                 int index7,
                                 int index8);

        static std::string ToTriplet(const std::vector<int>& board, int index0, int index1, int index2);

        static std::string ToCell(int value);

    public:
        static std::string ConvertToCode(const std::vector<int>& board);
    };
}
