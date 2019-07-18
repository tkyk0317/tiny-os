cmake_minimum_required(VERSION 3.5.1)

add_library(
    sys_call
    fork.cpp
    uart.cpp
)
