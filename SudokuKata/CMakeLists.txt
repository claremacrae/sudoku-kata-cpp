add_executable(SudokuKata
        main.cpp
        )
target_compile_features(SudokuKata PUBLIC cxx_std_17)
target_link_libraries(SudokuKata PUBLIC SudokuKataCpp)
