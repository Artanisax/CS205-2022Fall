cmake_minimum_required(VERSION 3.10)

project(Matrix)

aux_source_directory(./src DIR_SRCS)

include_directories(include)

add_executable(Matrix ${DIR_SRCS})
