#include "GridStringifier.h"

namespace SudokuKata::Utilities
{

    std::string GridStringifier::ConvertToString(const std::vector<int>& board)
    {
        const std::string divider = "+---+---+---+\n";
        auto result = divider;
        result += ToRowNumbered(board, 0);
        result += ToRowNumbered(board, 1);
        result += ToRowNumbered(board, 2);
        result += divider;
        result += ToRowNumbered(board, 3);
        result += ToRowNumbered(board, 4);
        result += ToRowNumbered(board, 5);
        result += divider;
        result += ToRowNumbered(board, 6);
        result += ToRowNumbered(board, 7);
        result += ToRowNumbered(board, 8);
        result += divider;
        return result;
    }

    std::string GridStringifier::ToRowNumbered(const std::vector<int>& board, int row)
    {
        auto o = row * 9;
        return ToRow(board, o + 0, o + 1, o + 2, o + 3, o + 4, o + 5, o + 6, o + 7, o + 8);
    }

    std::string GridStringifier::ToRow(const std::vector<int>& board,
                                       int index0,
                                       int index1,
                                       int index2,
                                       int index3,
                                       int index4,
                                       int index5,
                                       int index6,
                                       int index7,
                                       int index8)
    {
        std::string row = "|";
        row += ToTriplet(board, index0, index1, index2);
        row += "|";
        row += ToTriplet(board, index3, index4, index5);
        row += "|";
        row += ToTriplet(board, index6, index7, index8);
        row += "|";
        row += "\n";
        return row;
    }

    std::string
    GridStringifier::ToTriplet(const std::vector<int>& board, int index0, int index1, int index2)
    {
        auto value1 = board[index0];
        auto value2 = board[index1];
        auto value3 = board[index2];
        return ToCell(value1) + ToCell(value2) + ToCell(value3);
    }

    std::string GridStringifier::ToCell(int value)
    {
        return value == 0 ? "." : std::to_string(value);
    }

    std::string GridStringifier::ConvertToCode(const std::vector<int>& board)
    {
        std::string result;
        for (auto digit : board)
        {
            result += std::to_string(digit);
        }
        return result;
    }
}
