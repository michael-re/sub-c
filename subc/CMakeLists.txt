cmake_minimum_required(VERSION 3.20)
project(subc)

set(CMAKE_C_STANDARD               17)
set(CMAKE_C_EXTENSIONS             OFF)
set(CMAKE_C_STANDARD_REQUIRED      ON)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

set(CMAKE_BINARY_DIR       ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

set(msvc_compile_options
        /W4          # all reasonable warnings
        /w14242      # 'identifier': conversion from 'type1' to 'type1', possible loss of data
        /w14287      # 'operator': unsigned/negative constant mismatch
        /we4289      # nonstandard extension used: 'variable': loop control variable declared in the for-loop is used outside the for-loop scope
        /w14311      # 'variable': pointer truncation from 'type1' to 'type2'
        /w14545      # expression before comma evaluates to a function which is missing an argument list
        /w14546      # function call before comma missing argument list
        /w14555      # expression has no effect; expected expression with side- effect
        /w14619      # pragma warning: there is no warning number 'number'
        /w14826      # conversion from 'type1' to 'type_2' is sign-extended. This may cause unexpected runtime behavior.
        /permissive- # standards conformance mode for MSVC compiler.
        /WX
)

set(clang_compile_options
        -Wall
        -Wextra            # reasonable and standard
        -Wcast-align       # warn for potential performance problem casts
        -Wunused           # warn on anything being unused
        -Wpedantic         # warn if non-standard C is used
        -Wconversion       # warn on type conversions that may lose data
        -Wsign-conversion  # warn on sign conversions
        -Wnull-dereference # warn if a null dereference is detected
        -Wformat=2         # warn on security issues around functions that format output (ie printf)
        -Werror
)

set(gcc_compile_options
        ${clang_compile_options}
        -Wmisleading-indentation # warn if indentation implies blocks where blocks do not exist
        -Wduplicated-cond        # warn if if / else chain has duplicated conditions
        -Wduplicated-branches    # warn if if / else branches have duplicated code
        -Wlogical-op             # warn about logical operations being used where bitwise were probably wanted
)

if(MSVC)
        set(subc_compile_options ${msvc_compile_options})
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(subc_compile_options ${clang_compile_options})
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        set(subc_compile_options ${gcc_compile_options})
else()
        set(subc_compile_options "")
        message(AUTHOR_WARNING "no compiler warnings set for '${CMAKE_CXX_COMPILER_ID}' compiler.")
endif()

file(GLOB subc_inc_files include/subc/*.h)
file(GLOB subc_src_files source/subc/*.c)

add_executable(subc ${subc_src_files} ${subc_inc_files})
target_include_directories(subc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(subc PRIVATE ${subc_compile_options})
