#include "GroupsWithNMasks.h"

#include <fmt/format.h>

GroupsWithNMasks::GroupsWithNMasks(int mask,
                                   const std::string& description,
                                   const std::vector<CellGroup>& cells,
                                   const std::vector<CellGroup>& cellsWithMask,
                                   int cleanableCellsCount)
{
    Mask = mask;
    Description = description;
    Cells = cells;
    CellsWithMask = cellsWithMask;
    CleanableCellsCount = cleanableCellsCount;
}

std::string GroupsWithNMasks::ToString() const
{
    std::string result = fmt::format("{{ Mask = {0}, Description = {1}, Cells = {2}, "
                                     "CellsWithMask = {3}, CleanableCellsCount = {4}",
                                     Mask,
                                     Description,
                                     Cells.size(),
                                     CellsWithMask.size(),
                                     CleanableCellsCount);
    result += "\n   Cells:";
    for (const auto& item : Cells)
    {
        result += "\n      ";
        result += item.ToString();
    }
    result += "\n   CellsWithMask:";
    for (const auto& item : CellsWithMask)
    {
        result += "\n      ";
        result += item.ToString();
    }
    result += "\n}";
    return result;
}
