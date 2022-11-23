cmake_minimum_required(VERSION 3.10)

project(Matrix)

aux_source_directory(./src DIR_SRCS)

include_directories(include)

add_executable(Matrix ${DIR_SRCS})

add_compile_options(-Wall -O3 -fopenmp -mavx2 -march=native)