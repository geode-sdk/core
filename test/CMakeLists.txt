cmake_minimum_required(VERSION 3.8)

# Only grab the relative file names to put in the list.
file(GLOB_RECURSE TESTS RELATIVE "${CMAKE_CURRENT_LIST_DIR}" "*.cpp")
# Take out the cpp suffix from the tests.
string(REPLACE ".cpp" "" TESTS "${TESTS}")

foreach (TEST IN LISTS TESTS)
    # This is so that the meta tests aren't built unless we are targeting Windows.
    string(FIND "${TEST}" "meta" INDEX)
    # Wow, this sucks! Why can we not evaluate expressions
    if (NOT INDEX EQUAL -1)
        set(META_TEST TRUE)
    else()
        set(META_TEST FALSE)
    endif()
    if (META_TEST AND NOT WIN32)
        continue()
    endif()

    string(REPLACE "/" "." TARGET "${TEST}")
    message("Adding test ${TARGET}")
    
    add_executable("${TARGET}" "${TEST}.cpp")
    target_link_libraries("${TARGET}"
        geode::core::meta
        geode::core::hook)
    target_compile_features("${TARGET}" PRIVATE cxx_std_17)
    target_compile_definitions("${TARGET}" PRIVATE "TEST_NAME=\"${TARGET}\"")

    # This is so we target the right architecture for meta on Clang for Windows.
    if (META_TEST AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options("${TARGET}" PRIVATE "--target=i686-pc-windows-msvc")
        target_link_options("${TARGET}" PRIVATE "--target=i686-pc-windows-msvc")
    endif()
    add_test(NAME "${TARGET}" COMMAND "${TARGET}")
endforeach()
