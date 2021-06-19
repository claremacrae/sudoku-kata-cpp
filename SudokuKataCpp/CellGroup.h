#pragma once

#include <map>
#include <string>
#include <vector>

class CellGroup
{
public:
    int Discriminator = 0;
    std::string Description;
    int Index = 0;
    int Row = 0;
    int Column = 0;

public:
    CellGroup(int discriminator, const std::string& description, int index, int row, int column);

    std::string ToString() const;
};
