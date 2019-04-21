cmake_minimum_required(VERSION 3.5)
project(CompareVolume)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -static \
    -fno-optimize-sibling-calls -fno-strict-aliasing -D_LINUX \
    -lm -s -O2 -Wall -Wtype-limits -Wno-unknown-pragmas")

add_subdirectory (src) 
add_subdirectory (tests)

enable_testing ()
add_test (NAME UnitTests COMMAND Test)