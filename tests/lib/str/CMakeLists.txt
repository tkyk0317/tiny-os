cmake_minimum_required(VERSION 3.13.4)

add_test(
    NAME str-lib-test
    COMMAND $<TARGET_FILE:str-lib-test>
)

link_directories(../../cpputest/lib/)
add_executable(
    str-lib-test
    str.cpp
)

target_link_libraries(
    str-lib-test
    CppUTest
    CppUTestExt
)
