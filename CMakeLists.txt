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

# Enables debug mode for the given target(library or executable)
function(enable_debug target_name)
    target_compile_options(${target_name} PRIVATE -g)
    target_compile_definitions(${target_name} PRIVATE DEBUG DEBUG_LEVEL=2)
endfunction()

# Define a library 
function(create_lib library_name)
    add_library(${library_name} STATIC ${SOURCES})
    target_include_directories(${library_name} PUBLIC "${CMAKE_SOURCE_DIR}/include")
endfunction()

# Define a test executable for each tests/<test_name>.c file
function(create_tests)

    # Create a debug version of the library
    create_lib(lib_code_debug)
    target_compile_options(lib_code_debug PRIVATE -g)
    target_compile_definitions(lib_code_debug PRIVATE DEBUG DEBUG_LEVEL=2)

    # Get a list of all the '.c' files in the 'tests' directory
    file(GLOB_RECURSE TEST_SOURCES  
        RELATIVE "${CMAKE_SOURCE_DIR}"
        "${CMAKE_SOURCE_DIR}/tests/*.c"
    )

    # For each test source file
    foreach(TEST_SOURCE ${TEST_SOURCES})

        # Get the nAM absolute path of the test source file
        # get_filename_component(TEST_PATH ${TEST_SOURCE} ABSOLUTE)
        
        # Get the name of the test file
        get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

        message(STATUS "TEST_NAME: ${TEST_NAME}")
        message(STATUS "TEST_PATH: ${TEST_SOURCE}")

        # Create a executable for the given test file
        add_executable(
            ${TEST_NAME} 
            "${TEST_SOURCE}"
        )
    
        # Allow tests to import each other 
        target_include_directories(${TEST_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/tests")

        # Link the debug version of the library to the test executable
        target_link_libraries(${TEST_NAME} lib_code_debug)

        # Enable debug mode for the test executable
        target_compile_options(${TEST_NAME} PRIVATE -g)
        target_compile_definitions(${TEST_NAME} PRIVATE DEBUG DEBUG_LEVEL=2)

        # Link the math library to the test executable
        target_link_libraries(${TEST_NAME} m)

        # Print the test name
        message(STATUS "Creating test: ${TEST_NAME}")
    
    endforeach()

endfunction()


# Release version 
# Library
create_lib(lib_code_release)
# Executable
add_executable(main "main.c") #Main executable
target_link_libraries(main lib_code_release) # Link library to main executable
target_link_libraries(main m) # Link math library to main executable

# Debugging
# Create all the tests
create_tests()
