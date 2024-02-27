# CMakeList.txt : CMake project for cubemap, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project(cubemap)

set(CMAKE_CXX_STANDARD 11)

# Add source to this project's executable.
add_executable (cubemap "main.cpp" "solid_angle.h" "solid_angle.cpp")

message("${CMAKE_CURRENT_SOURCE_DIR}")

set(CENG_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../ceng-core")

message("CENG_ROOT=${CENG_ROOT}")

#set(CENG_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../ceng-core/inc")
set(CENG_INCLUDE_PATH "${CENG_ROOT}/inc")
set(CENG_LIB_INCLUDE_PATH "${CENG_INCLUDE_PATH}/lib")

set(CENG_LIB_PATH "${CENG_ROOT}/lib/win32/Release")

message("CENG_LIB_PATH=${CENG_LIB_PATH}")
message("CENG_INCLUDE_PATH=${CENG_INCLUDE_PATH}")
message("CENG_LIB_INCLUDE_PATH=${CENG_LIB_INCLUDE_PATH}")

target_include_directories(cubemap PRIVATE 
$<BUILD_INTERFACE:${CENG_INCLUDE_PATH}>
)

target_include_directories(cubemap PRIVATE 
$<BUILD_INTERFACE:${CENG_LIB_INCLUDE_PATH}>
)

target_link_libraries(cubemap
	${CENG_LIB_PATH}/timerlib.lib
)

message("INCLUDE_DIRECTORIES:")
message("${INCLUDE_DIRECTORIES}")
message("INTERFACE_INCLUDE_DIRECTORIES:")
message("${INTERFACE_INCLUDE_DIRECTORIES}")

# TODO: Add tests and install targets if needed.