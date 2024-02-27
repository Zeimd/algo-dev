# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("algo-dev")

message("CXX flags = {CXX}")

# Include sub-projects.
add_subdirectory ("division")
add_subdirectory ("fast_div")
add_subdirectory ("modulus")
add_subdirectory ("vector_sort")
add_subdirectory ("color_conversion")
add_subdirectory ("vector_search")
add_subdirectory ("trig")
add_subdirectory ("floating_point")
add_subdirectory ("cubemap")
