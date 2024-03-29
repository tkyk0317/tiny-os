include_directories(./cpputest/include ../include)

# C++ Version
set (CMAKE_CXX_STANDARD 17)

# build type
set(CMAKE_BUILD_TYPE Debug)

# x86上で実行するので、コンパイラやフラグを変更
set(CMAKE_ASM_COMPILER as)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_RANLIB ranlib)
set(CMAKE_LINKER gcc)
set(CMAKE_ASM_FLAGS "")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter -g")

add_test(
    NAME tiny-os-test
    COMMAND $<TARGET_FILE:tiny-os-test>
)

link_directories(./cpputest/lib/)
add_executable(
    tiny-os-test
    main.cpp
    scheduler.cpp
    stub.cpp
    ${CMAKE_SOURCE_DIR}/scheduler.cpp
    ${CMAKE_SOURCE_DIR}/devices/uart/uart.cpp
)

add_subdirectory(lib)
add_subdirectory(elf)

target_compile_options(
    tiny-os-test
    PRIVATE
    "-D__TEST__"
)

target_link_libraries(
    tiny-os-test
    CppUTest
    CppUTestExt
)
