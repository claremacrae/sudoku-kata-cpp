#pragma once

#include <string>
#include "CellGroup.h"

#include <vector>

class TwoDigitMaskGroups
{
public:
    const int Mask = 0;
    const int Discriminator = 0;
    const std::string Description;
    const std::vector<CellGroup> Cells;

public:
    TwoDigitMaskGroups(int mask,
                       int discriminator,
                       const std::string& description,
                       const std::vector<CellGroup>& cells);

    std::string ToString() const;
};
