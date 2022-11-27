cmake_minimum_required(VERSION 3.10)

project(Mul)

aux_source_directory(./src DIR_SRCS)

include_directories(include)
include_directories(/usr/local/OpenBLAS.OpenMP/include)

link_directories(/usr/local/OpenBLAS.OpenMP/lib)

add_compile_options(-Wall -fopenmp -mavx2 -O3)

add_executable(Mul ${DIR_SRCS})

find_package(OpenMP REQUIRED)
target_link_libraries(Mul PUBLIC OpenMP::OpenMP_C)

target_link_libraries(Mul PUBLIC /usr/local/OpenBLAS.OpenMP/lib/libopenblas.a)