cmake_minimum_required(VERSION 3.26)
project(ASVT C)

ADD_LIBRARY(sortLib STATIC quick_sort/quick_sort.c)
ADD_LIBRARY(pgmLib STATIC pgm_tools/pgm_tools.c)

set(CMAKE_C_STANDARD 11)

set(libs Stat)

add_executable(filter main.c)

target_link_libraries(filter sortLib pgmLib)