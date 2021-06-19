#include "Program.h"
#include "CellGroup.h"
#include "GroupsWithNMasks.h"
#include "Utilities/PortableRandomNumberGenerator.h"

#include "TwoDigitMaskGroups.h"

#include <fmt/format.h>
#include <algorithm>
#include <deque>
#include <iostream>
#include <map>
#include <numeric>
#include <stack>
#include <unordered_map>

namespace SudokuKata
{

    void Program::Play(PortableRandomNumberGenerator& rng, std::ostream& console)
    {
        // >>>> #region Construct fully populated board
        // Prepare empty board
        std::string line = "+---+---+---+";
        std::string middle = "|...|...|...|";
        std::vector<std::vector<char>> board = {std::vector<char>(line.begin(), line.end()),
                                                std::vector<char>(middle.begin(), middle.end()),
                                                std::vector<char>(middle.begin(), middle.end()),
                                                std::vector<char>(middle.begin(), middle.end()),
                                                std::vector<char>(line.begin(), line.end()),
                                                std::vector<char>(middle.begin(), middle.end()),
                                                std::vector<char>(middle.begin(), middle.end()),
                                                std::vector<char>(middle.begin(), middle.end()),
                                                std::vector<char>(line.begin(), line.end()),
                                                std::vector<char>(middle.begin(), middle.end()),
                                                std::vector<char>(middle.begin(), middle.end()),
                                                std::vector<char>(middle.begin(), middle.end()),
                                                std::vector<char>(line.begin(), line.end())};

        // Construct board to be solved

        // Top element is current state of the board
        std::stack<std::vector<int>> stateStack;

        // Top elements are (row, col) of cell which has been modified compared to previous state
        std::stack<int> rowIndexStack;
        std::stack<int> colIndexStack;

        // Top element indicates candidate digits (those with False) for (row, col)
        std::stack<std::vector<bool>> usedDigitsStack;

        // Top element is the value that was set on (row, col)
        std::stack<int> lastDigitStack;

        // Indicates operation to perform next
        // - expand - finds next empty cell and puts new state on stacks
        // - move - finds next candidate number at current pos and applies it to current state
        // - collapse - pops current state from stack as it did not yield a solution
        std::string command = "expand";
        while (stateStack.size() <= 9 * 9)
        {
            if (command == "expand")
            {
                std::vector<int> currentState(9 * 9);

                if (!stateStack.empty())
                {
                    std::copy_n(
                        stateStack.top().begin(), currentState.size(), currentState.begin());
                }

                int bestRow = -1;
                int bestCol = -1;
                std::vector<bool> bestUsedDigits;
                int bestCandidatesCount = -1;
                int bestRandomValue = -1;
                bool containsUnsolvableCells = false;

                for (int index = 0; index < currentState.size(); index++)
                {
                    if (currentState[index] == 0)
                    {

                        int row = index / 9;
                        int col = index % 9;
                        int blockRow = row / 3;
                        int blockCol = col / 3;

                        std::vector<bool> isDigitUsed(9);

                        for (int i = 0; i < 9; i++)
                        {
                            int rowDigit = currentState[9 * i + col];
                            if (rowDigit > 0)
                            {
                                isDigitUsed[rowDigit - 1] = true;
                            }

                            int colDigit = currentState[9 * row + i];
                            if (colDigit > 0)
                            {
                                isDigitUsed[colDigit - 1] = true;
                            }

                            int blockDigit =
                                currentState[(blockRow * 3 + i / 3) * 9 + (blockCol * 3 + i % 3)];
                            if (blockDigit > 0)
                            {
                                isDigitUsed[blockDigit - 1] = true;
                            }
                        } // for (i = 0..8)

                        auto candidatesCount =
                            std::count(isDigitUsed.begin(), isDigitUsed.end(), false);

                        if (candidatesCount == 0)
                        {
                            containsUnsolvableCells = true;
                            break;
                        }

                        int randomValue = rng.Next();

                        if (bestCandidatesCount < 0 || candidatesCount < bestCandidatesCount ||
                            (candidatesCount == bestCandidatesCount &&
                             randomValue < bestRandomValue))
                        {
                            bestRow = row;
                            bestCol = col;
                            bestUsedDigits = isDigitUsed;
                            bestCandidatesCount = candidatesCount;
                            bestRandomValue = randomValue;
                        }

                    } // for (index = 0..81)
                }

                if (!containsUnsolvableCells)
                {
                    stateStack.push(currentState);
                    rowIndexStack.push(bestRow);
                    colIndexStack.push(bestCol);
                    usedDigitsStack.push(bestUsedDigits);
                    lastDigitStack.push(0); // No digit was tried at this position
                }

                // Always try to move after expand
                command = "move";

            } // if (command == "expand")
            else if (command == "collapse")
            {
                stateStack.pop();
                rowIndexStack.pop();
                colIndexStack.pop();
                usedDigitsStack.pop();
                lastDigitStack.pop();

                command = "move"; // Always try to move after collapse
            }
            else if (command == "move")
            {

                int rowToMove = rowIndexStack.top();
                int colToMove = colIndexStack.top();
                int digitToMove = lastDigitStack.top();
                lastDigitStack.pop();

                int rowToWrite = rowToMove + rowToMove / 3 + 1;
                int colToWrite = colToMove + colToMove / 3 + 1;

                std::vector<bool>& usedDigits = usedDigitsStack.top();
                std::vector<int>& currentState = stateStack.top();
                int currentStateIndex = 9 * rowToMove + colToMove;

                int movedToDigit = digitToMove + 1;
                while (movedToDigit <= 9 && usedDigits[movedToDigit - 1])
                {
                    movedToDigit += 1;
                }

                if (digitToMove > 0)
                {
                    usedDigits[digitToMove - 1] = false;
                    currentState[currentStateIndex] = 0;
                    board[rowToWrite][colToWrite] = '.';
                }

                if (movedToDigit <= 9)
                {
                    lastDigitStack.push(movedToDigit);
                    usedDigits[movedToDigit - 1] = true;
                    currentState[currentStateIndex] = movedToDigit;
                    board[rowToWrite][colToWrite] = static_cast<char>('0' + movedToDigit);

                    // Next possible digit was found at current position
                    // Next step will be to expand the state
                    command = "expand";
                }
                else
                {
                    // No viable candidate was found at current position - pop it in the next iteration
                    lastDigitStack.push(0);
                    command = "collapse";
                }
            } // if (command == "move")
        }

        console << std::endl;
        console << "Final look of the solved board:" << std::endl;
        for (const auto& item : board)
        {
            for (const auto& character : item)
            {
                console << character;
            }
            console << '\n';
        }

        // >>>> #endregion

        // >>>> #region Generate initial board from the completely solved one
        // Board is solved at this point.
        // Now pick subset of digits as the starting position.
        int remainingDigits = 30;
        int maxRemovedPerBlock = 6;
        std::vector<std::vector<int>> removedPerBlock(3, std::vector<int>(3));
        std::vector<int> positions(9 * 9);
        std::iota(positions.begin(), positions.end(), 0);

        std::vector<int> state = stateStack.top();

        std::vector<int> finalState(state.size());
        std::copy_n(state.begin(), finalState.size(), finalState.begin());

        int removedPos = 0;
        while (removedPos < 9 * 9 - remainingDigits)
        {
            int curRemainingDigits = positions.size() - removedPos;
            int indexToPick = removedPos + rng.Next(curRemainingDigits);

            int row = positions[indexToPick] / 9;
            int col = positions[indexToPick] % 9;

            int blockRowToRemove = row / 3;
            int blockColToRemove = col / 3;

            if (removedPerBlock[blockRowToRemove][blockColToRemove] >= maxRemovedPerBlock)
            {
                continue;
            }

            removedPerBlock[blockRowToRemove][blockColToRemove] += 1;

            int temp = positions[removedPos];
            positions[removedPos] = positions[indexToPick];
            positions[indexToPick] = temp;

            int rowToWrite = row + row / 3 + 1;
            int colToWrite = col + col / 3 + 1;

            board[rowToWrite][colToWrite] = '.';

            int stateIndex = 9 * row + col;
            state[stateIndex] = 0;

            removedPos += 1;
        }

        console << std::endl;
        console << "Starting look of the board to solve:" << std::endl;
        for (const auto& item : board)
        {
            for (const auto& character : item)
            {
                console << character;
            }
            console << '\n';
        }
        // >>>> #endregion

        // >>>> #region Prepare lookup structures that will be used in further execution
        console << std::endl;
        console << std::string(80, '=') << std::endl;
        console << std::endl;

        std::map<int, int> maskToOnesCount;
        maskToOnesCount[0] = 0;
        for (int i = 1; i < (1 << 9); i++)
        {
            int smaller = i >> 1;
            int increment = i & 1;
            maskToOnesCount[i] = maskToOnesCount[smaller] + increment;
        }

        std::unordered_map<int, int> singleBitToIndex;
        for (int i = 0; i < 9; i++)
        {
            singleBitToIndex[1 << i] = i;
        }

        int allOnes = (1 << 9) - 1;
        // >>>> #endregion

        bool changeMade = true;
        while (changeMade)
        {
            changeMade = false;

            // >>>> #region Calculate candidates for current state of the board
            std::vector<int> candidateMasks(state.size());

            for (int i = 0; i < state.size(); i++)
            {
                if (state[i] == 0)
                {

                    int row = i / 9;
                    int col = i % 9;
                    int blockRow = row / 3;
                    int blockCol = col / 3;

                    int colidingNumbers = 0;
                    for (int j = 0; j < 9; j++)
                    {
                        int rowSiblingIndex = 9 * row + j;
                        int colSiblingIndex = 9 * j + col;
                        int blockSiblingIndex = 9 * (blockRow * 3 + j / 3) + blockCol * 3 + j % 3;

                        int rowSiblingMask = 1 << (state[rowSiblingIndex] - 1);
                        int colSiblingMask = 1 << (state[colSiblingIndex] - 1);
                        int blockSiblingMask = 1 << (state[blockSiblingIndex] - 1);

                        colidingNumbers =
                            colidingNumbers | rowSiblingMask | colSiblingMask | blockSiblingMask;
                    }

                    candidateMasks[i] = allOnes & ~colidingNumbers;
                }
            }
            // >>>> #endregion

            // >>>> #region Build a collection (named cellGroups) which maps cell indices into distinct groups (rows/columns/blocks)
            std::vector<std::vector<CellGroup>> cellGroups;
            int discriminator = 0;
            {
                for (int row = 0; row != 9; ++row)
                {
                    std::vector<CellGroup> groups;
                    for (int col = 0; col != 9; col++)
                    {
                        int index = row * 9 + col;
                        groups.push_back(CellGroup(
                            discriminator, fmt::format("row #{0}", row + 1), index, row, col));
                    }
                    cellGroups.push_back(groups);
                    discriminator += 1;
                }
            }

            {
                for (int col = 0; col != 9; col++)
                {
                    std::vector<CellGroup> groups;
                    for (int row = 0; row != 9; ++row)
                    {
                        int index = row * 9 + col;
                        groups.push_back(CellGroup(
                            discriminator, fmt::format("column #{0}", col + 1), index, row, col));
                    }
                    cellGroups.push_back(groups);
                    discriminator += 1;
                }
            }

            {
                for (int block_row = 0; block_row < 3; ++block_row)
                {
                    for (int block_col = 0; block_col < 3; ++block_col)
                    {
                        std::vector<CellGroup> groups;
                        for (int cell_number = 0; cell_number < 9; ++cell_number)
                        {
                            int row = (3 * block_row) + (cell_number / 3);
                            int col = (3 * block_col) + (cell_number % 3);
                            int index = row * 9 + col;
                            groups.push_back(CellGroup(
                                discriminator,
                                fmt::format("block ({0}, {1})", block_row + 1, block_col + 1),
                                index,
                                row,
                                col));
                        }
                        cellGroups.push_back(groups);
                        discriminator += 1;
                    }
                }
            }
            // >>>> #endregion

            bool stepChangeMade = true;
            while (stepChangeMade)
            {
                stepChangeMade = false;

                // >>>> #region Pick cells with only one candidate left

                std::vector<int> singleCandidateIndices;
                for (int index = 0; index < candidateMasks.size(); ++index)
                {
                    auto mask = candidateMasks[index];
                    auto bitsSet = maskToOnesCount[mask];
                    if (bitsSet == 1)
                    {
                        singleCandidateIndices.push_back(index);
                    }
                }

                if (!singleCandidateIndices.empty())
                {
                    int pickSingleCandidateIndex = rng.Next(singleCandidateIndices.size());
                    int singleCandidateIndex = singleCandidateIndices[pickSingleCandidateIndex];
                    int candidateMask = candidateMasks[singleCandidateIndex];
                    int candidate = singleBitToIndex[candidateMask];

                    int row = singleCandidateIndex / 9;
                    int col = singleCandidateIndex % 9;

                    int rowToWrite = row + row / 3 + 1;
                    int colToWrite = col + col / 3 + 1;

                    state[singleCandidateIndex] = candidate + 1;
                    board[rowToWrite][colToWrite] = static_cast<char>('1' + candidate);
                    candidateMasks[singleCandidateIndex] = 0;
                    changeMade = true;

                    console << "(" << row + 1 << ", " << col + 1 << ") can only contain "
                            << candidate + 1 << '.' << std::endl;
                }

                // >>>> #endregion

                // >>>> #region Try to find a number which can only appear in one place in a row/column/block

                if (!changeMade)
                {
                    std::vector<std::string> groupDescriptions;
                    std::vector<int> candidateRowIndices;
                    std::vector<int> candidateColIndices;
                    std::vector<int> candidates;

                    for (int digit = 1; digit <= 9; digit++)
                    {
                        int mask = 1 << (digit - 1);
                        for (int cellGroup = 0; cellGroup < 9; cellGroup++)
                        {
                            int rowNumberCount = 0;
                            int indexInRow = 0;

                            int colNumberCount = 0;
                            int indexInCol = 0;

                            int blockNumberCount = 0;
                            int indexInBlock = 0;

                            for (int indexInGroup = 0; indexInGroup < 9; indexInGroup++)
                            {
                                int rowStateIndex = 9 * cellGroup + indexInGroup;
                                int colStateIndex = 9 * indexInGroup + cellGroup;
                                int blockRowIndex = (cellGroup / 3) * 3 + indexInGroup / 3;
                                int blockColIndex = (cellGroup % 3) * 3 + indexInGroup % 3;
                                int blockStateIndex = blockRowIndex * 9 + blockColIndex;

                                if ((candidateMasks[rowStateIndex] & mask) != 0)
                                {
                                    rowNumberCount += 1;
                                    indexInRow = indexInGroup;
                                }

                                if ((candidateMasks[colStateIndex] & mask) != 0)
                                {
                                    colNumberCount += 1;
                                    indexInCol = indexInGroup;
                                }

                                if ((candidateMasks[blockStateIndex] & mask) != 0)
                                {
                                    blockNumberCount += 1;
                                    indexInBlock = indexInGroup;
                                }
                            }

                            if (rowNumberCount == 1)
                            {
                                groupDescriptions.push_back(fmt::format("Row #{0}", cellGroup + 1));
                                candidateRowIndices.push_back(cellGroup);
                                candidateColIndices.push_back(indexInRow);
                                candidates.push_back(digit);
                            }

                            if (colNumberCount == 1)
                            {
                                groupDescriptions.push_back(
                                    fmt::format("Column #{0}", cellGroup + 1));
                                candidateRowIndices.push_back(indexInCol);
                                candidateColIndices.push_back(cellGroup);
                                candidates.push_back(digit);
                            }

                            if (blockNumberCount == 1)
                            {
                                int blockRow = cellGroup / 3;
                                int blockCol = cellGroup % 3;

                                groupDescriptions.push_back(
                                    fmt::format("Block ({0}, {1})", blockRow + 1, blockCol + 1));
                                candidateRowIndices.push_back(blockRow * 3 + indexInBlock / 3);
                                candidateColIndices.push_back(blockCol * 3 + indexInBlock % 3);
                                candidates.push_back(digit);
                            }
                        } // for (cellGroup = 0..8)
                    }     // for (digit = 1..9)

                    if (!candidates.empty())
                    {
                        int index = rng.Next(candidates.size());
                        std::string description = groupDescriptions.at(index);
                        int row = candidateRowIndices.at(index);
                        int col = candidateColIndices.at(index);
                        int digit = candidates.at(index);
                        int rowToWrite = row + row / 3 + 1;
                        int colToWrite = col + col / 3 + 1;

                        std::string message = fmt::format("{0} can contain {1} only at ({2}, {3}).",
                                                          description,
                                                          digit,
                                                          row + 1,
                                                          col + 1);

                        int stateIndex = 9 * row + col;
                        state[stateIndex] = digit;
                        candidateMasks[stateIndex] = 0;
                        board[rowToWrite][colToWrite] = static_cast<char>('0' + digit);

                        changeMade = true;

                        console << message << std::endl;
                    }
                }

                // >>>> #endregion

                // >>>> #region Try to find pairs of digits in the same row/column/block and remove them from other colliding cells
                if (!changeMade)
                {
                    std::vector<int> twoDigitMasks;
                    std::copy_if(candidateMasks.begin(),
                                 candidateMasks.end(),
                                 std::back_inserter(twoDigitMasks),
                                 [&](int mask)
                                 {
                                     bool alreadySeen = std::find(twoDigitMasks.begin(),
                                                                  twoDigitMasks.end(),
                                                                  mask) != twoDigitMasks.end();
                                     return maskToOnesCount[mask] == 2 && (!alreadySeen);
                                 });

                    std::vector<TwoDigitMaskGroups> groups;
                    for (int mask : twoDigitMasks)
                    {
                        for (const auto& cellsInGroup : cellGroups)
                        {
                            int cellsMatchingMask =
                                std::count_if(cellsInGroup.begin(),
                                              cellsInGroup.end(),
                                              [&](const CellGroup& cell)
                                              { return (candidateMasks[cell.Index] == mask); });
                            if (cellsMatchingMask != 2)
                            {
                                continue;
                            }

                            bool anyValid =
                                std::any_of(cellsInGroup.begin(),
                                            cellsInGroup.end(),
                                            [&](const CellGroup& cell)
                                            {
                                                int cellMask = candidateMasks[cell.Index];
                                                return cellMask != mask && (cellMask & mask) > 0;
                                            });
                            if (!anyValid)
                            {
                                continue;
                            }

                            const CellGroup& front = cellsInGroup.front();
                            groups.push_back(TwoDigitMaskGroups(
                                mask, front.Discriminator, front.Description, cellsInGroup));
                        }
                    }

                    if (!groups.empty())
                    {
                        for (const TwoDigitMaskGroups& group : groups)
                        {
                            std::vector<CellGroup> cells;
                            for (const CellGroup& cell : group.Cells)
                            {
                                if (candidateMasks[cell.Index] != group.Mask &&
                                    (candidateMasks[cell.Index] & group.Mask) > 0)
                                {
                                    cells.push_back(cell);
                                }
                            }

                            std::vector<CellGroup> maskCells;
                            for (const CellGroup& cell : group.Cells)
                            {
                                if (candidateMasks[cell.Index] == group.Mask)
                                {
                                    maskCells.push_back(cell);
                                }
                            }

                            if (!cells.empty())
                            {
                                int upper = 0;
                                int lower = 0;
                                int temp = group.Mask;

                                int value = 1;
                                while (temp > 0)
                                {
                                    if ((temp & 1) > 0)
                                    {
                                        lower = upper;
                                        upper = value;
                                    }
                                    temp = temp >> 1;
                                    value += 1;
                                }

                                console << fmt::format("Values {0} and {1} in {2} are in "
                                                       "cells ({3}, {4}) and ({5}, {6}).",
                                                       lower,
                                                       upper,
                                                       group.Description,
                                                       maskCells[0].Row + 1,
                                                       maskCells[0].Column + 1,
                                                       maskCells[1].Row + 1,
                                                       maskCells[1].Column + 1)
                                        << std::endl;

                                for (const auto& cell : cells)
                                {
                                    int maskToRemove = candidateMasks[cell.Index] & group.Mask;
                                    std::vector<int> valuesToRemove;
                                    int curValue = 1;
                                    while (maskToRemove > 0)
                                    {
                                        if ((maskToRemove & 1) > 0)
                                        {
                                            valuesToRemove.push_back(curValue);
                                        }
                                        maskToRemove = maskToRemove >> 1;
                                        curValue += 1;
                                    }

                                    std::string valuesReport =
                                        fmt::format("{}", fmt::join(valuesToRemove, ", "));
                                    console << fmt::format("{0} cannot appear in ({1}, {2}).",
                                                           valuesReport,
                                                           cell.Row + 1,
                                                           cell.Column + 1)
                                            << std::endl;

                                    candidateMasks[cell.Index] &= ~group.Mask;
                                    stepChangeMade = true;
                                }
                            }
                        }
                    }
                }
                // >>>> #endregion

                // >>>> #region Try to find groups of digits of size N which only appear in N cells within row/column/block
                // When a set of N digits only appears in N cells within row/column/block, then no other digit can appear in the same set of cells
                // All other candidates can then be removed from those cells

                if (!changeMade && !stepChangeMade)
                {
                    std::vector<int> masks;
                    for (const auto& [mask, onesCount] : maskToOnesCount)
                    {
                        if (onesCount > 1)
                        {
                            masks.push_back(mask);
                        }
                    }

                    std::vector<GroupsWithNMasks> groupsWithNMasks;
                    for (const auto& mask : masks)
                    {
                        for (const auto& groups : cellGroups)
                        {
                            bool allMatching = std::all_of(
                                groups.begin(),
                                groups.end(),
                                [&](const CellGroup& cell) {
                                    return (state[cell.Index] == 0 ||
                                            (mask & (1 << (state[cell.Index] - 1))) == 0);
                                });
                            if (!allMatching)
                            {
                                continue;
                            }
                            std::vector<CellGroup> cellsWithMask;
                            for (const auto& cell : groups)
                            {
                                if (state[cell.Index] == 0 &&
                                    (candidateMasks[cell.Index] & mask) != 0)
                                {
                                    cellsWithMask.push_back(cell);
                                }
                            }
                            int cleanableCellsCount =
                                std::count_if(groups.begin(),
                                              groups.end(),
                                              [&](const CellGroup& cell)
                                              {
                                                  return state[cell.Index] == 0 &&
                                                         (candidateMasks[cell.Index] & mask) != 0 &&
                                                         (candidateMasks[cell.Index] & ~mask) != 0;
                                              });
                            if (cellsWithMask.size() == maskToOnesCount[mask])
                            {
                                GroupsWithNMasks groupWithNMasks(mask,
                                                                 groups.front().Description,
                                                                 groups,
                                                                 cellsWithMask,
                                                                 cleanableCellsCount);
                                groupsWithNMasks.push_back(groupWithNMasks);
                            }
                        }
                    }

                    for (auto groupWithNMasks : groupsWithNMasks)
                    {
                        int mask = groupWithNMasks.Mask;

                        if (std::count_if(groupWithNMasks.Cells.begin(),
                                          groupWithNMasks.Cells.end(),
                                          [&](auto cell) {
                                              return (candidateMasks[cell.Index] & mask) != 0 &&
                                                     (candidateMasks[cell.Index] & ~mask) != 0;
                                          }))
                        {
                            std::string message;
                            message += (fmt::format("In {0} values ", groupWithNMasks.Description));

                            std::string separator;
                            separator = "";
                            int temp = mask;
                            int curValue = 1;
                            while (temp > 0)
                            {
                                if ((temp & 1) > 0)
                                {
                                    message += (fmt::format("{0}{1}", separator, curValue));
                                    separator = ", ";
                                }
                                temp = temp >> 1;
                                curValue += 1;
                            }

                            message += (" appear only in cells");
                            for (const auto& cell : groupWithNMasks.CellsWithMask)
                            {
                                message +=
                                    (fmt::format(" ({0}, {1})", cell.Row + 1, cell.Column + 1));
                            }

                            message += (" and other values cannot appear in those cells.");

                            console << message << std::endl;
                        }

                        for (const auto& cell : groupWithNMasks.CellsWithMask)
                        {
                            int maskToClear = candidateMasks[cell.Index] & ~groupWithNMasks.Mask;
                            if (maskToClear == 0)
                            {
                                continue;
                            }

                            candidateMasks[cell.Index] &= groupWithNMasks.Mask;
                            stepChangeMade = true;

                            int valueToClear = 1;

                            std::string separator;
                            std::string message;

                            while (maskToClear > 0)
                            {
                                if ((maskToClear & 1) > 0)
                                {
                                    message += (fmt::format("{0}{1}", separator, valueToClear));
                                    separator = ", ";
                                }
                                maskToClear = maskToClear >> 1;
                                valueToClear += 1;
                            }

                            message += (fmt::format(" cannot appear in cell ({0}, {1}).",
                                                    cell.Row + 1,
                                                    cell.Column + 1));
                            console << message << std::endl;
                        }
                    }
                }

                // >>>> #endregion
            }

            // >>>> #region Final attempt - look if the board has multiple solutions
            if (!changeMade)
            {
                // This is the last chance to do something in this iteration:
                // If this attempt fails, board will not be entirely solved.

                // Try to see if there are pairs of values that can be exchanged arbitrarily
                // This happens when board has more than one valid solution

                std::deque<int> candidateIndex1;
                std::deque<int> candidateIndex2;
                std::deque<int> candidateDigit1;
                std::deque<int> candidateDigit2;

                for (int i = 0; i < candidateMasks.size() - 1; i++)
                {
                    if (maskToOnesCount[candidateMasks[i]] == 2)
                    {
                        int row = i / 9;
                        int col = i % 9;
                        int blockIndex = 3 * (row / 3) + col / 3;

                        int temp = candidateMasks[i];
                        int lower = 0;
                        int upper = 0;
                        for (int digit = 1; temp > 0; digit++)
                        {
                            if ((temp & 1) != 0)
                            {
                                lower = upper;
                                upper = digit;
                            }
                            temp = temp >> 1;
                        }

                        for (int j = i + 1; j < candidateMasks.size(); j++)
                        {
                            if (candidateMasks[j] == candidateMasks[i])
                            {
                                int row1 = j / 9;
                                int col1 = j % 9;
                                int blockIndex1 = 3 * (row1 / 3) + col1 / 3;

                                if (row == row1 || col == col1 || blockIndex == blockIndex1)
                                {
                                    candidateIndex1.push_back(i);
                                    candidateIndex2.push_back(j);
                                    candidateDigit1.push_back(lower);
                                    candidateDigit2.push_back(upper);
                                }
                            }
                        }
                    }
                }

                // At this point we have the lists with pairs of cells that might pick one of two digits each
                // Now we have to check whether that is really true - does the board have two solutions?

                std::vector<int> stateIndex1;
                std::vector<int> stateIndex2;
                std::vector<int> value1;
                std::vector<int> value2;

                while (!candidateIndex1.empty())
                {
                    int index1 = candidateIndex1.front();
                    candidateIndex1.pop_front();
                    int index2 = candidateIndex2.front();
                    candidateIndex2.pop_front();
                    int digit1 = candidateDigit1.front();
                    candidateDigit1.pop_front();
                    int digit2 = candidateDigit2.front();
                    candidateDigit2.pop_front();

                    std::vector<int> alternateState(finalState.size());
                    std::copy_n(state.begin(), alternateState.size(), alternateState.begin());

                    if (finalState[index1] == digit1)
                    {
                        alternateState[index1] = digit2;
                        alternateState[index2] = digit1;
                    }
                    else
                    {
                        alternateState[index1] = digit1;
                        alternateState[index2] = digit2;
                    }

                    // What follows below is a complete copy-paste of the solver which appears at the beginning of this method
                    // However, the algorithm couldn't be applied directly and it had to be modified.
                    // Implementation below assumes that the board might not have a solution.
                    stateStack = std::stack<std::vector<int>>();
                    rowIndexStack = std::stack<int>();
                    colIndexStack = std::stack<int>();
                    usedDigitsStack = std::stack<std::vector<bool>>();
                    lastDigitStack = std::stack<int>();

                    command = "expand";
                    while (command != "complete" && command != "fail")
                    {
                        if (command == "expand")
                        {
                            std::vector<int> currentState(9 * 9);

                            if (!stateStack.empty())
                            {
                                std::copy_n(stateStack.top().begin(),
                                            currentState.size(),
                                            currentState.begin());
                            }
                            else
                            {
                                std::copy_n(alternateState.begin(),
                                            currentState.size(),
                                            currentState.begin());
                            }

                            int bestRow = -1;
                            int bestCol = -1;
                            std::vector<bool> bestUsedDigits;
                            int bestCandidatesCount = -1;
                            int bestRandomValue = -1;
                            bool containsUnsolvableCells = false;

                            for (int index = 0; index < currentState.size(); index++)
                            {
                                if (currentState[index] == 0)
                                {

                                    int row = index / 9;
                                    int col = index % 9;
                                    int blockRow = row / 3;
                                    int blockCol = col / 3;

                                    std::vector<bool> isDigitUsed(9);

                                    for (int i = 0; i < 9; i++)
                                    {
                                        int rowDigit = currentState[9 * i + col];
                                        if (rowDigit > 0)
                                        {
                                            isDigitUsed[rowDigit - 1] = true;
                                        }

                                        int colDigit = currentState[9 * row + i];
                                        if (colDigit > 0)
                                        {
                                            isDigitUsed[colDigit - 1] = true;
                                        }

                                        int blockDigit = currentState[(blockRow * 3 + i / 3) * 9 +
                                                                      (blockCol * 3 + i % 3)];
                                        if (blockDigit > 0)
                                        {
                                            isDigitUsed[blockDigit - 1] = true;
                                        }
                                    } // for (i = 0..8)

                                    int candidatesCount =
                                        std::count(isDigitUsed.begin(), isDigitUsed.end(), false);

                                    if (candidatesCount == 0)
                                    {
                                        containsUnsolvableCells = true;
                                        break;
                                    }

                                    int randomValue = rng.Next();

                                    if (bestCandidatesCount < 0 ||
                                        candidatesCount < bestCandidatesCount ||
                                        (candidatesCount == bestCandidatesCount &&
                                         randomValue < bestRandomValue))
                                    {
                                        bestRow = row;
                                        bestCol = col;
                                        bestUsedDigits = isDigitUsed;
                                        bestCandidatesCount = candidatesCount;
                                        bestRandomValue = randomValue;
                                    }

                                } // for (index = 0..81)
                            }

                            if (!containsUnsolvableCells)
                            {
                                stateStack.push(currentState);
                                rowIndexStack.push(bestRow);
                                colIndexStack.push(bestCol);
                                usedDigitsStack.push(bestUsedDigits);
                                lastDigitStack.push(0); // No digit was tried at this position
                            }

                            // Always try to move after expand
                            command = "move";

                        } // if (command == "expand")
                        else if (command == "collapse")
                        {
                            stateStack.pop();
                            rowIndexStack.pop();
                            colIndexStack.pop();
                            usedDigitsStack.pop();
                            lastDigitStack.pop();

                            if (!stateStack.empty())
                            {
                                command = "move"; // Always try to move after collapse
                            }
                            else
                            {
                                command = "fail";
                            }
                        }
                        else if (command == "move")
                        {

                            int rowToMove = rowIndexStack.top();
                            int colToMove = colIndexStack.top();
                            int digitToMove = lastDigitStack.top();
                            lastDigitStack.pop();

                            int rowToWrite = rowToMove + rowToMove / 3 + 1;
                            int colToWrite = colToMove + colToMove / 3 + 1;

                            std::vector<bool>& usedDigits = usedDigitsStack.top();
                            std::vector<int>& currentState = stateStack.top();
                            int currentStateIndex = 9 * rowToMove + colToMove;

                            int movedToDigit = digitToMove + 1;
                            while (movedToDigit <= 9 && usedDigits[movedToDigit - 1])
                            {
                                movedToDigit += 1;
                            }

                            if (digitToMove > 0)
                            {
                                usedDigits[digitToMove - 1] = false;
                                currentState[currentStateIndex] = 0;
                                board[rowToWrite][colToWrite] = '.';
                            }

                            if (movedToDigit <= 9)
                            {
                                lastDigitStack.push(movedToDigit);
                                usedDigits[movedToDigit - 1] = true;
                                currentState[currentStateIndex] = movedToDigit;
                                board[rowToWrite][colToWrite] =
                                    static_cast<char>('0' + movedToDigit);

                                if (std::count(currentState.begin(), currentState.end(), 0) > 0)
                                {
                                    command = "expand";
                                }
                                else
                                {
                                    command = "complete";
                                }
                            }
                            else
                            {
                                // No viable candidate was found at current position - pop it in the next iteration
                                lastDigitStack.push(0);
                                command = "collapse";
                            }
                        } // if (command == "move")

                    } // while (command != "complete" && command != "fail")

                    if (command == "complete")
                    { // Board was solved successfully even with two digits swapped
                        stateIndex1.push_back(index1);
                        stateIndex2.push_back(index2);
                        value1.push_back(digit1);
                        value2.push_back(digit2);
                    }
                } // while (!candidateIndex1.empty())

                if (!stateIndex1.empty())
                {
                    int pos = rng.Next(stateIndex1.size());
                    int index1 = stateIndex1.at(pos);
                    int index2 = stateIndex2.at(pos);
                    int digit1 = value1.at(pos);
                    int digit2 = value2.at(pos);
                    int row1 = index1 / 9;
                    int col1 = index1 % 9;
                    int row2 = index2 / 9;
                    int col2 = index2 % 9;

                    std::string description;

                    if (index1 / 9 == index2 / 9)
                    {
                        description = fmt::format("row #{0}", index1 / 9 + 1);
                    }
                    else if (index1 % 9 == index2 % 9)
                    {
                        description = fmt::format("column #{0}", index1 % 9 + 1);
                    }
                    else
                    {
                        description = fmt::format("block ({0}, {1})", row1 / 3 + 1, col1 / 3 + 1);
                    }

                    state[index1] = finalState[index1];
                    state[index2] = finalState[index2];
                    candidateMasks[index1] = 0;
                    candidateMasks[index2] = 0;
                    changeMade = true;

                    for (int i = 0; i < state.size(); i++)
                    {
                        int tempRow = i / 9;
                        int tempCol = i % 9;
                        int rowToWrite = tempRow + tempRow / 3 + 1;
                        int colToWrite = tempCol + tempCol / 3 + 1;

                        board[rowToWrite][colToWrite] = '.';
                        if (state[i] > 0)
                        {
                            board[rowToWrite][colToWrite] = static_cast<char>('0' + state[i]);
                        }
                    }

                    console << fmt::format(
                                   "Guessing that {0} and {1} are arbitrary in {2} (multiple "
                                   "solutions): Pick {3}->({4}, {5}), {6}->({7}, {8}).",
                                   digit1,
                                   digit2,
                                   description,
                                   finalState[index1],
                                   row1 + 1,
                                   col1 + 1,
                                   finalState[index2],
                                   row2 + 1,
                                   col2 + 1)
                            << std::endl;
                }
            }
            // >>>> #endregion

            if (changeMade)
            {
                // >>>> #region Print the board as it looks after one change was made to it
                for (const auto& item : board)
                {
                    for (const auto& character : item)
                    {
                        console << character;
                    }
                    console << '\n';
                }

                std::string code;
                for (const auto& item : board)
                {
                    for (const auto& character : item)
                    {
                        if (character == '-' || character == '+' || character == '|')
                        {
                            continue;
                        }
                        code += character == '.' ? '0' : character;
                    }
                }

                console << "Code: " << code << std::endl;
                console << std::endl;
                // >>>> #endregion
            }
        }
        // return;
    }

}
