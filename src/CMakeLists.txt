cmake_minimum_required(VERSION 3.8)

add_library(lilac_hook SHARED "lilac_hook.cpp" "lilac_hook.h" "lilac_hook.hpp")
set_property(TARGET lilac_hook PROPERTY CXX_STANDARD 17)