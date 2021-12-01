#include "../../include/lilac/core/hook/hook.hpp"
#include "impl.hpp"

using namespace lilac::core;
using namespace lilac::core::hook;

hook::Handle LILAC_CALL lilac::core::hook::add(const void* address, const void* detour) {
	return HookManager::add_hook(address, detour);
}

bool LILAC_CALL lilac::core::hook::remove(hook::Handle handle) {
	return HookManager::remove_hook(handle);
}