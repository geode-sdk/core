// for making sure function declarations match easier
#include "lilac_hook.hpp"

#include <windows.h>
#include <iostream>
#include <string>

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
		node.replaced_byte = *reinterpret_cast<const uint8_t*>(address);
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

void* g_waiting_for_gateway = nullptr; // oh my god this is disgusting mat

long NTAPI HookManager::handler(EXCEPTION_POINTERS* info) {
	std::cout << "handler hit" << std::endl;
	if (info->ExceptionRecord->ExceptionCode == EXCEPTION_SINGLE_STEP) {
		// ignore this var its disgusting i know
		if (g_waiting_for_gateway) {
			const auto address = g_waiting_for_gateway;
			patch(address, {0xCC});
			HookNode& node = all_hooks[address];
			std::cout << info->ExceptionRecord->ExceptionAddress << " " << address << std::endl;
			size_t instruction_size = (size_t)info->ExceptionRecord->ExceptionAddress - (size_t)address;
			bool is_jump = false;
			if (instruction_size > 13) {
				std::cout << "size is " << instruction_size << " guessing its a jump/call" << std::endl;
				instruction_size = 5;
				is_jump = true;
			}
			std::cout << "single step hit, instruction size is " << instruction_size << std::endl;
			node.gateway = VirtualAlloc(nullptr, instruction_size + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
			std::cout << "gateway at: " << node.gateway << std::endl;
			auto gateway = reinterpret_cast<uintptr_t>(node.gateway);
			*reinterpret_cast<uint8_t*>(node.gateway) = node.replaced_byte;
			if (is_jump) {
				auto a = reinterpret_cast<intptr_t>(address);
				// dst - src - 5 = x
				// dst = x + src + 5
				// (x + src1 + 5) - src2 - 5
				// x + src1 + 5 - src2 - 5
				// x + src1 - src2
				*reinterpret_cast<intptr_t*>(gateway + 1) = *reinterpret_cast<intptr_t*>(a + 1) + a - gateway;
			} else if (instruction_size > 1) {
				memcpy((void*)(gateway + 1), (void*)((uintptr_t)(address) + 1), instruction_size - 1);
			}
			*reinterpret_cast<uint8_t*>(gateway + instruction_size) = 0xE9;
			// dst - src - 5
			*reinterpret_cast<intptr_t*>(gateway + instruction_size + 1) = (intptr_t)address + instruction_size - (gateway + instruction_size) - 5;
			if (!is_jump)
				info->ContextRecord->Eip = gateway + instruction_size;
		}
		return EXCEPTION_CONTINUE_EXECUTION;
	} else if (info->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT) {
		void* address = reinterpret_cast<void*>(info->ExceptionRecord->ExceptionAddress);
		if (all_hooks.find(address) != all_hooks.end()) {
			HookNode& node = all_hooks[address];
			if (node.in_hook) {
				if (node.index < node.hooks.size()) {
					std::cout << "going to the detour " << node.index << std::endl;
					info->ContextRecord->Eip = (DWORD)node.hooks[node.index++]->m_detour;
				} else {
					node.in_hook = false;
					node.index = 0;
					// somehow execute the regular function without going through the trap
					if (node.gateway) {
						std::cout << "going to the gateway" << std::endl;
						info->ContextRecord->Eip = (DWORD)node.gateway;
					} else {
						std::cout << "making the gateway, set the eflags" << std::endl;
						info->ContextRecord->EFlags |= 0x100; // pies favorite
						patch(address, {node.replaced_byte});
						g_waiting_for_gateway = address;
					}
				}
			}
			else {
				std::cout << "in_hook = true" << std::endl;
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