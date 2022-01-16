cmake_minimum_required(VERSION 3.8)

add_executable(lilac_hook_test "hook_test.cpp")
target_link_libraries(lilac_hook_test 
    lilac::core::hook)

target_compile_features(lilac_hook_test PRIVATE cxx_std_17)

add_executable(lilac_meta_test "meta_test.cpp")
target_link_libraries(lilac_meta_test 
    lilac::core::meta
    lilac::core::hook)

target_compile_features(lilac_meta_test PRIVATE cxx_std_17)
