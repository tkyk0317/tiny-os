add_test(
    NAME elf-test
    COMMAND $<TARGET_FILE:elf-test>
)

link_directories(../cpputest/lib/)
add_executable(
    elf-test
    loader.cpp
    ${CMAKE_SOURCE_DIR}/elf/loader.cpp
)

target_compile_options(
    elf-test
    PRIVATE
    "-D__TEST__"
)

target_link_libraries(
    elf-test
    CppUTest
    CppUTestExt
)
