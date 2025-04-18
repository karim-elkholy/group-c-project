cmake_minimum_required(VERSION 3.10)
project(AES C)

# Set C89/C90 standard for all projects
set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Enable CTest
include(CTest)

# Compiler flags applied to ALL targets
# Treat warnings as errors
add_compile_options(-Wall -Werror)
# Adhere strictly to the above specified C standard
# add_compile_options(-pedantic)

# Use all library files 
file(GLOB_RECURSE SOURCES
    "lib/*.c"
    "include/*.h"
)

# Enables debug mode 
function(enable_debug target_name)
    target_compile_options(${target_name} PRIVATE -g)
    target_compile_definitions(${target_name} PRIVATE DEBUG DEBUG_LEVEL_DEBUG=3)
endfunction()

# Define a library 
function(create_lib target_name)
    add_library(${target_name} STATIC ${SOURCES})
    target_include_directories(${target_name} PUBLIC "${CMAKE_SOURCE_DIR}/include")
endfunction()

# Link libraries shared by both release & debug versions
function(link_shared_libs target_name)
    target_link_libraries(${target_name}
        m #Math library
    )
endfunction()

# Two versions of library 
# Release version of the library
create_lib(lib_code_release)
# Debug version of the library
create_lib(lib_code_debug)

# Executables
add_executable(aes_main "main.c") #Main executable
add_executable(aes_debug "tests/test.c") #Test executable

# Enable debug mode for the debug versions of test & the library 
enable_debug(aes_debug)
enable_debug(lib_code_debug)

# Link libraries 
link_shared_libs(aes_main)
target_link_libraries(aes_main lib_code_release)
link_shared_libs(aes_debug)
target_link_libraries(aes_debug lib_code_debug)
