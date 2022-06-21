#ifndef GEODE_CORE_TEST_META_SHARED_HPP
#define GEODE_CORE_TEST_META_SHARED_HPP

#include <geode/core/meta/meta.hpp>
#include <iostream>
#include <string>

// This is kind of horrible but I need the specialization
template <auto, template <class, class...> class>
static constexpr int wrap = 0;

template <class Ret, class... Args, Ret(func)(Args...), template <class, class...> class Conv>
static constexpr auto wrap<func, Conv> = Conv<Ret, Args...>::template get_wrapper<func>();

// Easier printing. std::cout sucks normally.
template <class... Args>
static void print(Args&&... args) {
    (std::cout << ... << args);
}

template <class... Args>
static void println(Args&&... args) {
    print(args..., '\n');
}

using namespace geode::core::meta;

static void __stdcall dummy(int num) {
    println("Dummy: ", num);
}

// Implement this in your test file!
void test();

int main() {
    // TEST_NAME is provided in our CMakeLists file.
    println("Running test " TEST_NAME "...");
    test();
    return 0;
}

#endif /* GEODE_CORE_TEST_META_SHARED_HPP */