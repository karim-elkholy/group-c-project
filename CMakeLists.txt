cmake_minimum_required(VERSION 3.10)
project("Hospital" C)

# Enable C99
# set(CMAKE_C_STANDARD 99)

# Compiler flags applied to ALL targets
# Taken from lab1
# C Code must confirm to 1989 C standard
# Tutor and Anna from Ed allow C99
# add_compile_options(-ansi)
# For development we can use gnu89
# This allows // comments 
add_compile_options(-ansi)
# Treat warnings as errors
add_compile_options(-Wall -Werror)
# Adhere strictly to the above specified C standard
# add_compile_options(-pedantic)

# Use all library files 
file(GLOB_RECURSE SOURCES
    "lib/**/*.c"
    "include/**/*.h"
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
add_executable(main "main.c") #Main executable
add_executable(encryption_test "tests/test_encryption.c") #Test encryption executable
add_executable(compression_test "tests/test_compression.c") #Test compression executable
add_executable(database_test "tests/test_database.c") #Test database executable

# Enable debug mode for the debug versions for tests & the debug library
enable_debug(encryption_test)
enable_debug(compression_test)
enable_debug(database_test)
enable_debug(lib_code_debug)

# Link custom library to the release & debug targets
target_link_libraries(main lib_code_release)
target_link_libraries(encryption_test lib_code_release)
target_link_libraries(compression_test lib_code_release)
target_link_libraries(database_test lib_code_release)
# Link math library to all targets
target_link_libraries(main lib_code_release m)
target_link_libraries(encryption_test lib_code_release m)
target_link_libraries(compression_test lib_code_release m)
target_link_libraries(database_test lib_code_release m)