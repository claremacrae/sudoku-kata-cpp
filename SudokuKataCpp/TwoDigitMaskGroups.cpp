#include "TwoDigitMaskGroups.h"
#include <fmt/format.h>
#include <cassert>

TwoDigitMaskGroups::TwoDigitMaskGroups(int mask,
                                       int discriminator,
                                       const std::string& description,
                                       const std::vector<CellGroup>& cells)
    : Mask(mask), Discriminator(discriminator), Description(description), Cells(cells)
{
    assert(cells.size() == 9);
}

std::string TwoDigitMaskGroups::ToString() const
{
    auto result = fmt::format("{0}: {1}, {2}: {3}, {4}: {5}\n",
                              "Mask",
                              Mask,
                              "Discriminator",
                              Discriminator,
                              "Description",
                              Description);
    for (const auto& cell : Cells)
    {
        result += fmt::format("   {0}\n", cell.ToString());
    }
    return result;
}
