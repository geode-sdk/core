cmake_minimum_required(VERSION 3.8)

add_library(lilac_hook STATIC
	"lilac_hook.cpp"
	"impl/lilac_hook_impl.cpp" 
	"impl/lilac_hook_windows.cpp")
set_property(TARGET lilac_hook PROPERTY CXX_STANDARD 17)
target_include_directories(lilac_hook PRIVATE "${CMAKE_SOURCE_DIR}/include")