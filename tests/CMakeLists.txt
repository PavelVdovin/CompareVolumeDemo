file(GLOB tests_SRC "*.cpp")
file(COPY assets DESTINATION .)

include_directories (
	../src
)

add_executable (Test ${tests_SRC})
target_link_libraries (Test CompareVolumeLib ${LMDB_LIBRARIES})
