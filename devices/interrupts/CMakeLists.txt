cmake_minimum_required(VERSION 3.5.1)

add_library(
    interrupts
    uart.cpp
    system_timer.cpp
)
