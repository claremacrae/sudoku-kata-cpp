#include "Program.h"
#include "Utilities/PortableRandomNumberGenerator.h"

int main()
{
    PortableRandomNumberGenerator random;
    SudokuKata::Program::Play(random);
}
