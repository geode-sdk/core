cmake_minimum_required(VERSION 3.8)

if (APPLE)
	set(PLATFORM "macos")
elseif (WIN32)
	set(PLATFORM "windows")
endif()

add_library(geode_core_hook STATIC
	"hook.cpp"
	"impl.cpp" 
	"${PLATFORM}.cpp")
add_library(geode::core::hook ALIAS geode_core_hook)

target_link_libraries(geode_core_hook
    geode::core::meta
)
target_compile_features(geode_core_hook PUBLIC cxx_std_17)
target_include_directories(geode_core_hook 
	PRIVATE
		${CMAKE_SOURCE_DIR}/include/geode/core/hook
	INTERFACE
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/geode>
)
