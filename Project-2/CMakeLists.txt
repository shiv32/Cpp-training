cmake_minimum_required(VERSION 4.0)
project(Project2_string_view VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main main.cpp)

target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/include)

if (MSVC)
  target_compile_options(main PRIVATE /W4 /permissive-)
else()
  target_compile_options(main PRIVATE -Wall -Wextra -Wpedantic)
endif()
