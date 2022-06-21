#include "opt.hpp"

#include "shared.hpp"

void test() {
    float f1 = 0.123f;
    float f2 = 3.123f;
    __asm {
        movss xmm0, f1
        movss xmm3, f2
    }
    /* Unfortunately, we can't test the registers accurately. This is because we can't rely on the
    compiler to generate assembly to preserve the registers we want in our own code. However, this
    is a non-issue in real-world use, because we won't be the callers, it will be the binary that we
    want to interface with, which we expect will already have well-behaving assembly.
    Expected:
    ecx: [random]
    edx: [random]
    xmm0: 0.123
    xmm1: clobbered (ecx)
    xmm2: clobbered (stack 0)
    xmm3: 3.123
    stack 0: x
    stack 1: 1337.3
    stack 2: He
    stack 3: llo
    */
    auto ret = wrap<opt, x86::Optcall>({ 'x' }, 1337.3f, "He", "llo");
    // Expected: "1337"
    println("Wrapper: ", ret);
}