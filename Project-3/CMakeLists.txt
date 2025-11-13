cmake_minimum_required(VERSION 3.10)
project(optional_implementation CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

# Add executable
add_executable(test_optional test_optional.cpp)

# Include directories
target_include_directories(test_optional PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Enable testing
enable_testing()
add_test(NAME optional_tests COMMAND test_optional)
