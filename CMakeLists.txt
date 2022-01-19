cmake_minimum_required(VERSION 3.8)

project(geode_core LANGUAGES C CXX)

add_subdirectory("src/hook")
add_subdirectory("src/meta")

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	add_subdirectory("test")
endif()

target_include_directories(
	geode_core_hook INTERFACE 
	"include/geode"
	"include/geode/core"
)
