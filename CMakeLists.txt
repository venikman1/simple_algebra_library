cmake_minimum_required(VERSION 3.14)
project(salib)

set(CMAKE_CXX_STANDARD 14)

include_directories(headers)

add_executable(salib
        src/main.cpp
        src/tests.cpp)

target_link_libraries(salib gmp)
target_link_libraries(salib gmpxx)
target_link_libraries(salib pthread)