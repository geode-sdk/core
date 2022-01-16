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


int main() {
    meta::Hook<&to_hook, &hook1, meta::x86::Optcall> hook;
    meta::Function<int(int, int, int), meta::x86::Optcall> f1 = test1;
    int val = f1(2, 3, 4);

    meta::Function<void(float, float, float, float, int, int, int), meta::x86::Optcall> f2 = test1;
    f2(6.0f, 2.0f, 3.0f, 5.0f, 2234, 2, 234);

    to_hook(6);
    to_hook(24);
}