cmake_minimum_required(VERSION 3.10)
project(AES C)

# Compiler flags applied to ALL targets
# Taken from lab1
# C Code must confirm to 1989 C standard
add_compile_options(-ansi)
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
    target_compile_definitions(${target_name} PRIVATE DEBUG DEBUG_LEVEL=2)
endfunction()

# Define a library 
function(create_lib target_name)
    add_library(${target_name} STATIC ${SOURCES})
    target_include_directories(${target_name} PUBLIC "${CMAKE_SOURCE_DIR}/include")
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

# Link custom library to the release & debug targets
target_link_libraries(aes_main lib_code_release)
target_link_libraries(aes_debug lib_code_debug)

# Link math library to all targets
link_libraries(m)
