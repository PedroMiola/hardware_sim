cmake_minimum_required(VERSION 3.10)

# Project name
project(HardwareSimulator)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Source files (excluding simulator.cpp)
file(GLOB SRC_FILES src/*.cpp)
list(REMOVE_ITEM SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/simulator.cpp)

# Add the main executable (simulator.cpp)
add_executable(simulator src/simulator.cpp)

# Test executables
file(GLOB TEST_FILES tests/*.cpp)
foreach(TEST_FILE ${TEST_FILES})
    # Extract the test name from the file name
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_FILE} ${SRC_FILES})
    target_include_directories(${TEST_NAME} PRIVATE include)
endforeach()

# Enable testing
enable_testing()
foreach(TEST_FILE ${TEST_FILES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()