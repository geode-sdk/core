// for making sure function declarations match easier
#include "lilac_hook.hpp"

#include <windows.h>
#include <iostream>
#include <string>
#include <Zydis/Zydis.h>

using namespace lilac;

inline void patch(void* loc, const std::vector<std::uint8_t>& bytes) {
    auto size = bytes.size();
    DWORD old_prot;
    VirtualProtect(loc, size, PAGE_EXECUTE_READWRITE, &old_prot);
    memcpy(loc, bytes.data(), size);
    VirtualProtect(loc, size, old_prot, &old_prot);
}

const HookHandle* LILAC_CALL HookManager::add_hook(void* const address, const void* detour) {
	if (!initialized) {
		initialized = AddVectoredExceptionHandler(true, handler);
	}

	HookHandle* ret = new HookHandle;
	ret->m_address = address;
	ret->m_detour = detour;

	if (all_hooks.find(address) == all_hooks.end()) {
		HookNode node;
		// this isnt used atm
		node.replaced_byte = *reinterpret_cast<const uint8_t*>(address);
		std::vector<uint8_t> gateway_bytes;
		ZydisDecoder decoder;
		if (!ZYAN_SUCCESS(ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_COMPAT_32, ZYDIS_ADDRESS_WIDTH_32))) {
			// well fuck
			return nullptr;
		}
		// uhh just hardcode the size to 32, i mean how likely it is that the function is smaller than that
		ZydisDecodedInstruction instruction;
		if (!ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, address, 32, &instruction))) {
			// fuck again
			return nullptr;
		}
		if (instruction.attributes & ZYDIS_ATTRIB_IS_RELATIVE) {
			// TODO
		} else {
			const auto iter = reinterpret_cast<uint8_t*>(address);
			gateway_bytes.assign(iter, iter + instruction.length);
		}
		node.gateway = VirtualAlloc(nullptr, gateway_bytes.size() + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		gateway_bytes.push_back(0xE9);
		// (dst + len) - (src + len) - 5
		// dst + len - src - len - 5
		// dst - src - 5
		intptr_t jump_dest = reinterpret_cast<intptr_t>(address) - reinterpret_cast<intptr_t>(node.gateway) - 5;
		const auto oh_god_wtf = reinterpret_cast<const uint8_t*>(&jump_dest);
		gateway_bytes.push_back(oh_god_wtf[0]);
		gateway_bytes.push_back(oh_god_wtf[1]);
		gateway_bytes.push_back(oh_god_wtf[2]);
		gateway_bytes.push_back(oh_god_wtf[3]);
		memcpy(node.gateway, gateway_bytes.data(), gateway_bytes.size());
		patch(address, {0xCC});
		all_hooks[address] = node;
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
	if (info->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT) {
		void* address = reinterpret_cast<void*>(info->ExceptionRecord->ExceptionAddress);
		if (all_hooks.find(address) != all_hooks.end()) {
			HookNode& node = all_hooks[address];
			if (node.in_hook) {
				if (node.index < node.hooks.size()) {
					info->ContextRecord->Eip = (DWORD)node.hooks[node.index++]->m_detour;
				} else {
					node.in_hook = false;
					node.index = 0;
					info->ContextRecord->Eip = (DWORD)node.gateway;
				}
			}
			else {
				node.in_hook = true;
			}
		}
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	return EXCEPTION_CONTINUE_SEARCH;
}

extern "C" const LilacHookHandle* LILAC_CALL lilac_add_hook(void* const address, const void* detour) {
	return HookManager::add_hook(address, detour);
}

extern "C" bool LILAC_CALL lilac_remove_hook(const LilacHookHandle* handle) {
	return HookManager::remove_hook(handle);
}