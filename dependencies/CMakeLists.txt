include(FetchContent)

# -------------------------------------------------------------------
# fmt
set(CATCH_BUILD_TESTING OFF CACHE BOOL "")
FetchContent_Declare(fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG "7.1.3")
FetchContent_MakeAvailable(fmt)
