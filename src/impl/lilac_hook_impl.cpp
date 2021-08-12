#include "lilac_hook_impl.hpp"

using namespace lilac;
using namespace impl;

namespace {
	struct Handle {
		const void* address;
		const void* detour;
	};
}

void HookManager::add_trap(const void* address, char buffer[]) {
	void* addr = const_cast<void*>(address);

	if (buffer != nullptr) {
		Platform<PLATFORM>::write_memory(buffer, addr, Platform<PLATFORM>::get_trap_size());
	}

	Platform<PLATFORM>::write_memory(addr, Platform<PLATFORM>::get_trap(), Platform<PLATFORM>::get_trap_size());
}

void HookManager::remove_trap(const void* address, char buffer[]) {
	Platform<PLATFORM>::write_memory(const_cast<void*>(address), buffer, Platform<PLATFORM>::get_trap_size());
}

bool HookManager::find_in_hooks(Exception& info) {
	auto pair = all_hooks.find(info.address);
	if (pair != all_hooks.end()) {
		auto& hook = pair->second;

		// add a frame hook for cleanup
		auto& frame = all_frames[info.return_address];
		frame.parent = &hook;
		frame.address = info.address;
		add_trap(info.return_address, frame.original_bytes);
		hook.frames.push_back(&frame);

		// redirect to next detour
		*info.instruction_pointer = hook.detours[hook.frames.size() - 1];
			
		// specialization for last detour: original calls don't call next detour
		if (hook.frames.size() == hook.detours.size()) {
			remove_trap(info.address, hook.original_bytes);
		}

		return true;
	}
	else {
		return false;
	}
}

bool HookManager::find_in_frames(Exception& info) {
	auto pair = all_frames.find(info.address);
	if (pair != all_frames.end()) {
		auto& frame = pair->second;
		auto& hook = *frame.parent;

		// specialization for last frame: need to replace trap
		if (hook.frames.size() == hook.detours.size()) {
			add_trap(frame.address, nullptr);
		}

		// remove the frame from the respective lists
		remove_trap(info.address, frame.original_bytes);
		hook.frames.pop_back();
		all_frames.erase(pair);

		return true;
	}
	else {
		return false;
	}
}

bool HookManager::handler(Exception& info) {
	if (find_in_hooks(info) || find_in_frames(info)) {
		return true;
	}
	else {
		return false;
	}
}

HookHandle HookManager::add_hook(const void* address, const void* detour) {
	auto& hook = all_hooks[address];
	auto& detours = hook.detours;
	auto i = std::find(detours.begin(), detours.end(), detour);
	if (i != detours.end()) {
		/* we're not allowing one detour to be assigned
		* in the hook chain multiple times.
		*/
		return nullptr;
	}
	else {
		if (detours.empty()) {
			// add trap instruction if this is the first detour to be added.
			add_trap(address, hook.original_bytes);
		}
			
		detours.push_back(detour);

		const auto ret = new Handle{ address, detour };
		return ret;
	}
}

bool HookManager::remove_hook(HookHandle handle) {
	auto& real = *static_cast<const Handle*>(handle);
	auto pair = all_hooks.find(real.address);
	if (pair != all_hooks.end()) {
		auto& detours = pair->second.detours;
		auto detour = std::find(detours.begin(), detours.end(), real.detour);
		if (detour != detours.end()) {
			detours.erase(detour);
			if (detours.empty()) {
				remove_trap(real.address, pair->second.original_bytes);
			}
			delete &real;
			return true;
		}
	}

	return false;
}