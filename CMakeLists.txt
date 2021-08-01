cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)

project(lilac_core LANGUAGES C CXX)

add_subdirectory(src)

project(coretest)

# cmake is hard
add_executable(coretest test/main.cpp src/lilac_hook.cpp)
target_include_directories(coretest PRIVATE src/)

add_custom_target(coretest-run COMMAND coretest)