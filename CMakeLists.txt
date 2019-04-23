cmake_minimum_required(VERSION 3.5)
project(CompareVolumeDemo)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

find_path(LMDB_INCLUDE_DIR NAMES  lmdb.h PATHS "$ENV{LMDB_DIR}/include")
find_library(LMDB_LIBRARIES NAMES lmdb PATHS "$ENV{LMDB_DIR}/lib" )

MESSAGE( STATUS "LMDB_INCLUDE_DIR: " ${LMDB_INCLUDE_DIR} )
MESSAGE( STATUS "LMDB_LIBRARIES: " ${LMDB_LIBRARIES} )

add_subdirectory (src) 
add_subdirectory (tests)

enable_testing ()
add_test (NAME UnitTests COMMAND Test)

file(GLOB demo_SRC "demo/*.cpp")

include_directories (
	src/
)

add_executable (CompareVolumeDemo ${demo_SRC})
target_link_libraries (CompareVolumeDemo CompareVolumeLib ${LMDB_LIBRARIES})