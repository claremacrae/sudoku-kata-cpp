add_executable(SudokuKataTestsCpp
        main.cpp
        GridStringifierTests.cpp
        PortableRandomNumberGeneratorTests.cpp
        ProgramTests.cpp
        )

target_link_libraries(SudokuKataTestsCpp SudokuKataCpp ApprovalTests::ApprovalTests Catch2::Catch2)

add_test(NAME SudokuKataTestsCpp COMMAND SudokuKataTestsCpp)
