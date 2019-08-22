add_library(
    usr
    dummy_func.cpp
)

add_executable(
    sample-app1
    sample_app1.cpp
    ${CMAKE_SOURCE_DIR}/sys/uart.s
)

add_executable(
    sample-app2
    sample_app2.cpp
    ${CMAKE_SOURCE_DIR}/sys/uart.s
)

