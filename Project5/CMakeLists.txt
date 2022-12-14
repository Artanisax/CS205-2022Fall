cmake_minimum_required(VERSION 3.10)

project(Matrix)

aux_source_directory(./src DIR_SRCS)

include_directories(include)

add_compile_options(-fopenmp)

add_executable(Matrix ${DIR_SRCS})

find_package(OpenMP REQUIRED)
target_link_libraries(Matrix PUBLIC OpenMP::OpenMP_C)
