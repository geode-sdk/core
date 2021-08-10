#include "lilac_hook_impl.hpp"

namespace lilac::impl {
	struct Handle {
		const void* address;
		const void* detour;
	};

	void HookManager::write_memory(void* to, const void* from, size_t size) {
		DWORD old;
		VirtualProtect(to, size, PAGE_EXECUTE_READWRITE, &old);
		WriteProcessMemory(GetCurrentProcess(), to, from, size, nullptr);
		VirtualProtect(to, size, old, &old);
	}

	void HookManager::add_trap(const void* address, char buffer[]) {
		void* addr = const_cast<void*>(address);

		if (buffer != nullptr) {
			write_memory(buffer, addr, sizeof(trap));
		}
		
		write_memory(addr, trap, sizeof(trap));
	}

	void HookManager::remove_trap(const void* address, char buffer[]) {
		write_memory(const_cast<void*>(address), buffer, sizeof(trap));
	}

	bool HookManager::find_in_hooks(EXCEPTION_POINTERS* info) {
		auto address = reinterpret_cast<void*>(info->ExceptionRecord->ExceptionAddress);
		auto return_address = *reinterpret_cast<void**>(info->ContextRecord->Esp);

		auto pair = all_hooks.find(address);
		if (pair != all_hooks.end()) {
			auto& hook = pair->second;
			if (!hook.active) {
				// initialize the hook chain
				hook.active = true;
				// clear the returns stack and set it up maybe?
			}

			// add a frame hook for cleanup
			auto& frame = all_frames[return_address];
			frame.parent = &hook;
			frame.address = address;
			add_trap(return_address, frame.original_bytes);
			hook.frames.push_back(&frame);

			// redirect to next detour
			info->ContextRecord->Eip = reinterpret_cast<DWORD>(hook.detours[hook.frames.size() - 1]);
			
			// specialization for last detour: original calls don't call next detour
			if (hook.frames.size() == hook.detours.size()) {
				remove_trap(address, hook.original_bytes);
			}

			return true;
		}
		else {
			return false;
		}
	}

	bool HookManager::find_in_frames(EXCEPTION_POINTERS* info) {
		auto return_address = reinterpret_cast<void*>(info->ExceptionRecord->ExceptionAddress);

		auto pair = all_frames.find(return_address);
		if (pair != all_frames.end()) {
			auto& frame = pair->second;
			auto& hook = *frame.parent;

			// specialization for last frame: need to replace trap
			if (hook.frames.size() == hook.detours.size()) {
				add_trap(frame.address, nullptr);
			}

			// remove the frame from the respective lists
			remove_trap(return_address, frame.original_bytes);
			hook.frames.pop_back();
			all_frames.erase(pair);

			return true;
		}
		else {
			return false;
		}
	}

	long _stdcall HookManager::handler(EXCEPTION_POINTERS* info) {
		if (info->ExceptionRecord->ExceptionCode == EXCEPTION_ILLEGAL_INSTRUCTION) {
			if (find_in_hooks(info) || find_in_frames(info)) {
				return EXCEPTION_CONTINUE_EXECUTION;
			}
		}

		return EXCEPTION_CONTINUE_SEARCH;
	}

	HookHandle HookManager::add_hook(const void* address, const void* detour) {
		static bool init = false;

		if (!init) {
			init = AddVectoredExceptionHandler(true, HookManager::handler);
		}

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
}