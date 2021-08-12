// for making sure function declarations match easier
#include <windows.h>
#include <vector>
#include <map>
#include <stack>

#include "lilac_hook.hpp"
#include "impl/lilac_hook_impl.hpp"

using namespace lilac;

HookHandle LILAC_CALL Hooks::add(const void* address, const void* detour) {
	return impl::HookManager::add_hook(address, detour);
}

bool LILAC_CALL Hooks::remove(HookHandle handle) {
	return impl::HookManager::remove_hook(handle);
}

extern "C" LilacHookHandle LILAC_CALL lilac_add_hook(const void* address, const void* detour) {
	return impl::HookManager::add_hook(address, detour);
}

extern "C" bool LILAC_CALL lilac_remove_hook(LilacHookHandle handle) {
	return impl::HookManager::remove_hook(handle);
}