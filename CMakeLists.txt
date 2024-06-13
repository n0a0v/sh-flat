cmake_minimum_required(VERSION 3.10)
project(sh-flat VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" CACHE PATH "Directory to place executables.")

add_subdirectory(benchmarks)
add_subdirectory(googletest)
add_subdirectory(tests)
