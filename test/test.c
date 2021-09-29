#include "lilac_hook.h"

#if defined(_WIN32)
	#include "minhook/include/MinHook.h"
	#include <Windows.h>
	#define NOINLINE __declspec(noinline)
#elif
	#define NOINLINE __attribute__((noinline))
#endif

#include <stdio.h>

// you see, on release builds this function got inlined
// meaning none of the hooks would work at all
void NOINLINE to_hook(const char* str) {
	printf("[STATUS]: %s\n", str);
}

void hook3(const char* str) {
	static int times = 0;
	switch (times) {
	case 0:
		to_hook("0");
		break;
	case 1:
		to_hook("1");
		break;
	case 2:
		to_hook("2");
		break;
	default:
		to_hook("max");
		break;
	}
	to_hook(str);
	++times;
}

void hook2(const char* str) {
	to_hook("hook2");
}

void hook1(const char* str) {
	for (unsigned int i = 0; i < 5; ++i) {
		to_hook(str);
	}
}

void(*minhook_gate)(const char*) = NULL;
void minhook_hook(const char* str) {
	minhook_gate("minhook");
	minhook_gate(str);
}

int main() {
	#if defined(_WIN32)
		MH_Initialize();

		MH_CreateHook(to_hook, minhook_hook, (void**)&minhook_gate);
		MH_EnableHook(to_hook);
	#endif

	typedef LilacHookHandle h;

	h h1 = lilac_add_hook(to_hook, hook1);
	h h2 = lilac_add_hook(to_hook, hook2);
	h h3 = lilac_add_hook(to_hook, hook3);

	to_hook("main");

	printf("\n--removing hook1--\n\n");
	lilac_remove_hook(h1);

	to_hook("main");

	printf("\n--removing all--\n\n");
	lilac_remove_hook(h2);
	lilac_remove_hook(h3);

	printf("removing invalid hook returned: %s\n\n", (lilac_remove_hook(h1) ? "true" : "false"));

	to_hook("main");
}
