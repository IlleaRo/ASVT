cmake_minimum_required(VERSION 3.26)
project(ASVT C)

set(CMAKE_C_STANDARD 11)

add_executable(ASVT main.c
        quick_sort/quick_sort.h
        quick_sort/quick_sort.c)
