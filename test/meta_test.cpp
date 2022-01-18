#include <lilac/core/meta/function.hpp>
#include <lilac/core/meta/hook.hpp>
#include <lilac/core/meta/optcall.hpp>
#include <iostream>

using namespace lilac::core;

int test1(int x) {
    std::cout << "Hi " << x << '\n';
    return 2;
}

int __fastcall to_hook(int x) {
    std::cout << x << ": to_hook\n";
    return x - 3;
}

int hook1(int x) {
    std::cout << "HACKED!!! u left urself logged in at the apple store\n";
    return to_hook(x + 6);
}

void optcall_test(float a, int b, int c, float d, float e) {
    std::cout << "xmm0 " << a << std::endl;
    std::cout << "edx " << b << std::endl;
    std::cout << "stack 0 " << c << std::endl;
    std::cout << "xmm3 " << d << std::endl;
    std::cout << "stack 1 " << e << std::endl;
}


int main() {
    meta::Hook<&to_hook, &hook1, meta::x86::Optcall> hook;
    meta::Function<int(int, int, int), meta::x86::Optcall> f1 = test1;
    int val = f1(2, 3, 4);

    meta::Function<void(float, float, float, float, int, int, int), meta::x86::Optcall> f2 = test1;
    f2(6.0f, 2.0f, 3.0f, 5.0f, 2234, 2, 234);

    to_hook(6);
    to_hook(24);

    auto wrapper = meta::Hook<&to_hook, &optcall_test, meta::x86::Optcall>::get_wrapper();
    // expected:
    // xmm0 0.123
    // edx 123
    // stack 0 420
    // xmm3 3.123
    // stack 1 1337
    wrapper(0.123f, 1907.f, 1908.f, 3.123f, 1909.f, 1910.f, 2337, 123, 420, 1337.f);
}