#ifndef __LILAC_HOOK_IMPL_HPP__
#define __LILAC_HOOK_IMPL_HPP__

#include "../lilac_hook.hpp"
#include <vector>
#include <map>

namespace lilac::impl {
	class HookManager {
	private:
		static constexpr char trap[] = { '\x0F', '\x0B' };

	private:
		struct CallFrame;

		struct HookChain {
			std::vector<const void*> detours = {};
			std::vector<CallFrame*> frames = {};
			bool active = false;
			char original_bytes[sizeof(trap)] = { 0 };
		};

		struct CallFrame {
			const void* address = nullptr;
			HookChain* parent = nullptr;
			char original_bytes[sizeof(trap)] = { 0 };
		};

	private:
		static inline std::map<const void*, HookChain> all_hooks = {};
		static inline std::map<const void*, CallFrame> all_frames = {};

	private:
		static void write_memory(void* to, const void* from, size_t size);

		/* these don't check char buffer bounds. it should have sizeof(trap) size.
		* pass nullptr to add_trap if you don't want to save the old bytes into a buffer.
		*/
		static void add_trap(const void* address, char buffer[]);
		static void remove_trap(const void* address, char buffer[]);

		static bool find_in_hooks(EXCEPTION_POINTERS* info);
		static bool find_in_frames(EXCEPTION_POINTERS* info);

		static long _stdcall handler(EXCEPTION_POINTERS* info);

	public:
		static HookHandle LILAC_CALL add_hook(const void* address, const void* detour);
		static bool LILAC_CALL remove_hook(HookHandle handle);
	};
}

#endif /* __LILAC_HOOK_IMPL_HPP__ */