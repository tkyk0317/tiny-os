cmake_minimum_required(VERSION 3.5.1)

add_library(
    sys_call
    sys_table.cpp
    fork.cpp
    uart.cpp
    uart.s
    fork.s
)
