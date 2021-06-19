#include "Program.h"
#include "Utilities/PortableRandomNumberGenerator.h"

#include <iostream>

int main()
{
    PortableRandomNumberGenerator random;
    SudokuKata::Program::Play(std::cout, random);
}
