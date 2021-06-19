#include "CellGroup.h"
#include <fmt/format.h>

CellGroup::CellGroup(
    int discriminator, const std::string& description, int index, int row, int column)
    : Discriminator(discriminator), Description(description), Index(index), Row(row), Column(column)
{
}

std::string CellGroup::ToString() const
{
    return fmt::format("{0}: {1}, {2}: {3}, {4}: {5}, {6}: {7}, {8}: {9}",
                       "Discriminator",
                       Discriminator,
                       "Description",
                       Description,
                       "Index",
                       Index,
                       "Row",
                       Row,
                       "Column",
                       Column);
}
