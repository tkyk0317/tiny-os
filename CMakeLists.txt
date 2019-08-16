cmake_minimum_required(VERSION 3.13.4)
#set(CMAKE_VERBOSE_MAKEFILE 1)

project(TinyOS)

# enable assembly
enable_language(ASM)

# configuration compoler
set(CROSS_COMPILE "aarch64-linux-gnu-")
set(CMAKE_ASM_COMPILER ${CROSS_COMPILE}as)
set(CMAKE_C_COMPILER ${CROSS_COMPILE}gcc)
set(CMAKE_CXX_COMPILER ${CROSS_COMPILE}g++)
set(CMAKE_RANLIB ${CROSS_COMPILE}ranlib)
set(CMAKE_LINKER ${CROSS_COMPILE}gcc)

# C++ Version
set (CMAKE_CXX_STANDARD 17)

# build type
set(CMAKE_BUILD_TYPE Debug)

# configuration of flags
set(CMAKE_ASM_FLAGS "-mcpu=cortex-a53")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -nostdlib -mcpu=cortex-a53 -fpic -ffreestanding -Wno-ignored-qualifiers -g")
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# include path
include_directories(./include)

# target files
add_executable(tiny-os
    boot.s
    vector.s
    context.s
    utils.s
    utils.cpp
    fork.cpp
    scheduler.cpp
    start.cpp
    handler.cpp
    memory.cpp
    shell.cpp
)

# sub directories
add_subdirectory(devices)
add_subdirectory(sys)
add_subdirectory(usr)
add_subdirectory(lib)

# link library
target_link_libraries(
    tiny-os
    usr
    str
    uart
    system_timer
    interrupts
    sys_call
)

# set linker script
set(LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld)
target_link_options(
    tiny-os
    PRIVATE
    "-T${LINKER_FILE}"
)

# enable test
enable_testing()
add_subdirectory(tests)

