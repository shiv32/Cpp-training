cmake_minimum_required(VERSION 3.20)
project(AnyLibrary CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(any_test main.cpp)
target_include_directories(any_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
