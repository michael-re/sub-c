cmake_minimum_required(VERSION 3.20)
project(subc)

set(CMAKE_C_STANDARD          17)
set(CMAKE_C_EXTENSIONS        OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_BINARY_DIR       ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

file(GLOB subc_inc_files include/subc/*.h)
file(GLOB subc_src_files source/subc/*.c)

add_executable(subc ${subc_src_files} ${subc_inc_files})
target_include_directories(subc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
