// for making sure function declarations match easier
#include "lilac_hook.hpp"

#include <windows.h>

using namespace lilac;

const HookHandle* LILAC_CALL HookManager::add_hook(const void* address, const void* detour) {
	if (!initialized) {
		initialized = AddVectoredExceptionHandler(true, handler);
	}

	HookHandle* ret = new HookHandle;
	ret->m_address = address;
	ret->m_detour = detour;

	if (all_hooks.find(address) == all_hooks.end()) {
		all_hooks[address] = {};
	}
	all_hooks[address].hooks.push_back(ret);

	return ret;
}

bool LILAC_CALL HookManager::remove_hook(const HookHandle* handle) {
	if (handle == nullptr) return false;

	if (all_hooks.find(handle->m_address) != all_hooks.end()) {
		auto& hooks = all_hooks[handle->m_address].hooks;
		auto i = std::find(hooks.begin(), hooks.end(), handle);
		if (i != hooks.end()) {
			delete *i;
			hooks.erase(i);
			return true;
		}
	}

	return false;
}

long NTAPI HookManager::handler(EXCEPTION_POINTERS* info) {
	void* address = reinterpret_cast<void*>(info->ExceptionRecord->ExceptionAddress);
	if (all_hooks.find(address) != all_hooks.end()) {
		HookNode& node = all_hooks[address];
		if (node.in_hook) {
			if (node.index < node.hooks.size()) {

			}
		}
		else {
			node.in_hook = true;
		}
	}
	return EXCEPTION_CONTINUE_SEARCH;
}

extern "C" const LilacHookHandle* LILAC_CALL lilac_add_hook(const void* address, const void* detour) {
	return HookManager::add_hook(address, detour);
}

extern "C" bool LILAC_CALL lilac_remove_hook(const LilacHookHandle* handle) {
	return HookManager::remove_hook(handle);
}