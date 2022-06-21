#include "shared.hpp"

static std::string thiscall(int i1, float f1) {
    println("ecx: ", i1);
    println("stack 0: ", f1);
    return "This " + std::to_string(f1);
}

void test() {
    /*
    Expected:
    ecx: 222
    stack 0: 341.2
    */
    auto ret = wrap<thiscall, x86::Thiscall>(222, 341.2f);
    // Expected: "This 341.2"
    println("Wrapper: ", ret);
}