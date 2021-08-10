#include <lilac_hook.hpp>
#include <iostream>
#include <string>

// msvc is stupid, this entire file is temporary anyways
#pragma optimize("", off)
void foobar() {
    std::cout << "i am foobar()" << std::endl;
}

void foobar_H() {
    std::cout << "i am hooking foobar()" << std::endl;
    foobar();
}

void foobar_H2() {
    std::cout << "i am also hooking foobar()" << std::endl;
    foobar();
}

void multibar() {
    std::cout << "i am multibar()" << std::endl;
}

void multibar_H() {
    std::cout << "i am hooking multibar() and will call it 3 times" << std::endl;
    multibar();
    multibar();
    multibar();
}

int main() {
    foobar();
    lilac::HookManager::add_hook(foobar, foobar_H);
    foobar();
    foobar();
    lilac::HookManager::add_hook(foobar, foobar_H2);
    foobar();
    foobar();
    
    // this causes an infinite loop
    // lilac::HookManager::add_hook(multibar, multibar_H);
    // multibar();
}
#pragma optimize("", on)