cmake_minimum_required(VERSION 3.29)

project(RTWeekend VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(RTWeekend main.cpp)

add_executable(PI pi.cpp)

target_include_directories(RTWeekend PUBLIC
                           "$(PROJECT_BINARY_DIR)")