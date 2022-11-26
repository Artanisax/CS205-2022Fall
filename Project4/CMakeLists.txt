cmake_minimum_required(VERSION 3.10)

project(Mul)

aux_source_directory(./src DIR_SRCS)

include_directories(include)

add_executable(Mul ${DIR_SRCS})

find_package(OpenMP REQUIRED)
target_link_libraries(Mul PUBLIC OpenMP::OpenMP_C)


add_compile_options(-Wall -O3 -fopenmp -march=native)