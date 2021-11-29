cmake_minimum_required(VERSION 3.8)

add_executable(lilac_hook_test "test.cpp")
target_link_libraries(lilac_hook_test 
    lilac::core::hook)

target_compile_features(lilac_hook_test PRIVATE cxx_std_17)
