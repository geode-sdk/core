#include "shared.hpp"

void test() {
    // Expected: "Dummy: 4"
    Function<void(int, std::string, int, float, int, float, bool), x86::Optcall> {
        dummy
    }(2, "lol", 2, 3.2f, 4, 5.6f, false);

    // Expected: "Dummy: 234"
    Function<void(float, float, float, float, int, int, int), x86::Optcall> {
        dummy
    }(6.0f, 2.0f, 3.0f, 5.0f, 2234, 2, 234);
}