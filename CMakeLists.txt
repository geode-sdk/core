cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)

# cmake is hard

project(coretest)

option(ZYDIS_BUILD_TOOLS "" OFF)
option(ZYDIS_BUILD_EXAMPLES "" OFF)
add_subdirectory("include/zydis")
add_executable(coretest test/main.cpp src/lilac_hook.cpp)
target_include_directories(coretest PRIVATE src/ include/zydis/include)
target_link_libraries(coretest PRIVATE Zydis)

add_custom_target(coretest-run COMMAND coretest)