add_test(
    NAME str-lib-test
    COMMAND $<TARGET_FILE:str-lib-test>
)

link_directories(../../cpputest/lib/)
add_executable(
    str-lib-test
    str.cpp
    ${CMAKE_SOURCE_DIR}/lib/str/str.c
)

target_link_libraries(
    str-lib-test
    CppUTest
    CppUTestExt
)
