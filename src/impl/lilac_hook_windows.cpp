#include "lilac_hook_windows.hpp"
#include "lilac_hook_impl.hpp"

#include <Windows.h>

using namespace lilac;
using namespace impl;

namespace {
	long __stdcall handler(EXCEPTION_POINTERS* info) {
		const void** ptr = reinterpret_cast<const void**>(&info->ContextRecord->Eip);
		Exception exception = {
			info->ExceptionRecord->ExceptionAddress,
			 *reinterpret_cast<void**>(info->ContextRecord->Esp),
			 ptr
		};

		if (HookManager::handler(exception)) {
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		else {
			return EXCEPTION_CONTINUE_SEARCH;
		}
	}
}

void WindowsX86::write_memory(void* to, const void* from, size_t size) {
	DWORD old;
	VirtualProtect(to, size, PAGE_EXECUTE_READWRITE, &old);
	WriteProcessMemory(GetCurrentProcess(), to, from, size, nullptr);
	VirtualProtect(to, size, old, &old);
}

bool WindowsX86::initialize() {
	return AddVectoredExceptionHandler(true, handler);
}