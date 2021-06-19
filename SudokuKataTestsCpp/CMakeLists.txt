add_executable(SudokuKataTestsCpp
        main.cpp
        PortableRandomNumberGeneratorTests.cpp
        )

target_link_libraries(SudokuKataTestsCpp SudokuKataCpp ApprovalTests::ApprovalTests Catch2::Catch2)

add_test(NAME SudokuKataTestsCpp COMMAND SudokuKataTestsCpp)
