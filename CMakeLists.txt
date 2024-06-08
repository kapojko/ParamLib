cmake_minimum_required(VERSION 3.12)
project(ParamLib_Test C)

# This CMakeLists.txt is for testing purposes only.
add_compile_definitions(TESTING)

# Collect all source files
file(GLOB SOURCES *.c)
add_executable(ParamLib_Test ${SOURCES})

# Platform-specific handling for linking against the math library
if (WIN32)
    # Windows: No need to link against "m" library
else ()
    # Linux: Link against the math library ("-lm")
    target_link_libraries(ParamLib_Test m)
endif ()

# Setup testing
enable_testing()
include(CTest)

add_test(
    NAME ParamLib_UnitTest
    COMMAND ParamLib_Test
)
