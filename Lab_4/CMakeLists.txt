cmake_minimum_required(VERSION 3.26)
project(Lab_4)

set(CMAKE_CXX_STANDARD 17)

include_directories(${MPI_INCLUDE_PATH})
find_package(MPI REQUIRED)

add_executable(Lab_4 main.cpp)

target_link_libraries(Lab_4 PUBLIC MPI::MPI_CXX)
