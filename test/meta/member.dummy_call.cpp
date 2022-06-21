#include "shared.hpp"

void test() {
    // Expected: "Dummy: 455"
    Function<void(void*, float, float, int), x86::Membercall> { dummy }(nullptr, 23.0f, 52.0f, 455);
}