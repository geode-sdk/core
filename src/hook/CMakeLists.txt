cmake_minimum_required(VERSION 3.8)

if (APPLE)
	set(PLATFORM "macos")
elseif (WIN32)
	set(PLATFORM "windows")
endif()

add_library(lilac_core_hook STATIC
	"hook.cpp"
	"impl.cpp" 
	"${PLATFORM}.cpp")
add_library(lilac::core::hook ALIAS lilac_core_hook)

target_link_libraries(lilac_core_hook
    lilac::core::meta
)
target_compile_features(lilac_core_hook PUBLIC cxx_std_17)
target_include_directories(lilac_core_hook 
	PRIVATE
		${CMAKE_SOURCE_DIR}/include/lilac/core
	INTERFACE
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/lilac>
)
