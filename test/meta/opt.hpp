#include "shared.hpp"

struct Small {
    char value;
};

static int
    opt(std::string s1, float f1, std::string s2, int i1, Small sm1, float f2, int i2, float f3) {
    println("ecx: ", i1);
    println("edx: ", i2);
    println("xmm0: ", f1);
    println("xmm1: clobbered (ecx)");
    println("xmm2: clobbered (stack 0)");
    println("xmm3: ", f2);
    println("stack 0: ", sm1.value);
    println("stack 1: ", f3);
    println("stack 2: ", s1);
    println("stack 3: ", s2);

    return static_cast<int>(f3);
}