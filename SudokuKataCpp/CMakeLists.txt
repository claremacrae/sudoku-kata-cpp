add_library(SudokuKataCpp
        CellGroup.cpp
        CellGroup.h
        GroupsWithNMasks.cpp
        GroupsWithNMasks.h
        Program.cpp
        Program.h
        TwoDigitMaskGroups.cpp
        TwoDigitMaskGroups.h
        Utilities/PortableRandomNumberGenerator.cpp
        Utilities/PortableRandomNumberGenerator.h
        )
target_compile_features(SudokuKataCpp PUBLIC cxx_std_17)
target_include_directories(SudokuKataCpp
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        )
target_link_libraries(SudokuKataCpp PUBLIC fmt::fmt)
