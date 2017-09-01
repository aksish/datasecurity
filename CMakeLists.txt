cmake_minimum_required(VERSION 3.8)
project(datasecurity)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c FileUtils.h StatUtils.h)
add_executable(datasecurity ${SOURCE_FILES})