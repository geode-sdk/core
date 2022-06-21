#include "member.hpp"

#include "shared.hpp"

void test() {
    /*
    Expected:
    ecx: 3
    xmm0: clobbered (ecx)
    xmm1: clobbered (stack 0)
    xmm2: clobbered (stack 1)
    xmm3: 1908.6
    stack 0: 555
    stack 1: 666
    stack 2: 777.2
    stack 3: Goo
    stack 4: dbye
    */
    auto ret = wrap<member, x86::Membercall>(
        // xmm0, xmm1,  xmm2,    xmm3,    xmm4,   xmm5, ecx, edx, stack...
        69.0f, 2333.0f, 1333.0f, 1908.6f, 222.0f, 223.0f, 3, 45, 555, 666, 777.2f, "Goo", "dbye"
    );
    // Expected: "Member 666"
    println("Wrapper: ", ret);
}