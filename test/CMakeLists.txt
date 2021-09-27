cmake_minimum_required(VERSION 3.8)

add_executable(lilac_hook_test
	"test.c")
target_link_libraries(lilac_hook_test
	lilac_hook)
target_include_directories(lilac_hook_test PRIVATE
	"${CMAKE_SOURCE_DIR}/include")
