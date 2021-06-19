#pragma once

#include "CellGroup.h"

#include <string>
#include <vector>

class GroupsWithNMasks
{
public:
    int Mask = 0;
    std::string Description;
    std::vector<CellGroup> Cells;
    std::vector<CellGroup> CellsWithMask;
    int CleanableCellsCount = 0;

public:
    GroupsWithNMasks(int mask,
                     const std::string& description,
                     const std::vector<CellGroup>& cells,
                     const std::vector<CellGroup>& cellsWithMask,
                     int cleanableCellsCount);

    std::string ToString() const;
};
