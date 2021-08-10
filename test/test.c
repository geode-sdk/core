#include "lilac_hook.h"
#include "minhook/include/MinHook.h"

#include <stdio.h>

void to_hook(const char* str) {
	printf("%s\n", str);
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
	printf("hi from minhook!\n");
	minhook_gate(str);
}

int main() {
	MH_Initialize();

	MH_CreateHook(to_hook, minhook_hook, (void**)&minhook_gate);
	MH_EnableHook(to_hook);

	lilac_add_hook(to_hook, hook1);
	lilac_add_hook(to_hook, hook2);
	lilac_add_hook(to_hook, hook3);

	to_hook("main");
}