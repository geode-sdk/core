cmake_minimum_required(VERSION 3.8)

if(APPLE)
	add_library(lilac_hook STATIC
		"lilac_hook.cpp"
		"impl/lilac_hook_impl.cpp" 
		"impl/lilac_hook_macos.cpp")
elseif(WIN32)
	add_library(lilac_hook STATIC
		"lilac_hook.cpp"
		"impl/lilac_hook_impl.cpp" 
		"impl/lilac_hook_windows.cpp")
endif()
set_property(TARGET lilac_hook PROPERTY CXX_STANDARD 17)
target_include_directories(lilac_hook PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../include)
