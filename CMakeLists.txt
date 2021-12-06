cmake_minimum_required(VERSION 3.8)

project(lilac_core LANGUAGES C CXX)

add_subdirectory("src/hook")
add_subdirectory("src/meta")

set(MAIN_PROJECT OFF)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	set(MAIN_PROJECT ON)
endif()

if (MAIN_PROJECT)
	add_subdirectory("test")
endif()

target_include_directories(
	lilac_core_hook INTERFACE 
	"include/lilac"
	"include/lilac/core"
)
