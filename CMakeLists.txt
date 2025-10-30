cmake_minimum_required(VERSION 3.15)
project(HardwareSimulator CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Core sources (exclude the app entry point file that contains main)
file(GLOB SRC_CORE CONFIGURE_DEPENDS src/*.cpp)
list(REMOVE_ITEM SRC_CORE ${CMAKE_CURRENT_SOURCE_DIR}/src/simulator.cpp)

# Build core static library from components, platform, etc.
add_library(hardware_core STATIC ${SRC_CORE})
target_include_directories(hardware_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Build the application executable that contains main
add_executable(simulator src/simulator.cpp)
target_link_libraries(simulator PRIVATE hardware_core)
target_include_directories(simulator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Tests link against the same core library
enable_testing()
file(GLOB TEST_FILES CONFIGURE_DEPENDS tests/*.cpp)
foreach(TEST_FILE ${TEST_FILES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_FILE})
    target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(${TEST_NAME} PRIVATE hardware_core)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()