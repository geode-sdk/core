cmake_minimum_required(VERSION 3.8)

add_library(lilac_core_meta INTERFACE)
add_library(lilac::core::meta ALIAS lilac_core_meta)

target_compile_features(lilac_core_meta INTERFACE cxx_std_17)
target_include_directories(
	lilac_core_meta
	INTERFACE
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)