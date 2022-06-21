#include "shared.hpp"

static std::string
    member(int i1, std::string s1, int i2, int i3, float f1, std::string s2, float f2) {
    println("ecx: ", i1);
    println("xmm0: clobbered (ecx)");
    println("xmm1: clobbered (stack 0)");
    println("xmm2: clobbered (stack 1)");
    println("xmm3: ", f1);
    println("stack 0: ", i2);
    println("stack 1: ", i3);
    println("stack 2: ", f2);
    println("stack 3: ", s1);
    println("stack 4: ", s2);
    return "Member " + std::to_string(i3);
}