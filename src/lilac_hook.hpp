#ifndef __LILAC_HOOK_HPP__
#define __LILAC_HOOK_HPP__

#include "lilac_hook.h"

#include <vector>
#include <map>

#include <windows.h>

namespace lilac {
	using HookHandle = LilacHookHandle;

	class HookManager {
	public:
		/*
		* params:
		* address - a pointer to the virtual memory to be hooked.
		* detour - a pointer to the function to redirect the instruction pointer to.
		*
		* returns:
		* null if failed.
		* a valid, constant pointer to a hook handle if succeeded.
		*
		* notes:
		* do not modify the returned pointer if valid. doing so will result in
		* undefined behavior.
		*
		* the hookee should have the same calling convention and parameters
		* as the detour. otherwise, crashing is almost certain to occur.
		*/
		static const HookHandle* LILAC_CALL add_hook(void* const address, const void* detour);

		/*
		* params:
		* handle - a pointer to the hook handle to be removed.
		*
		* returns:
		* true if the hook was successfully removed.
		* false if removal failed.
		*/
		static bool LILAC_CALL remove_hook(const HookHandle* handle);

	private:
		// node that represents the state of the hooks at a certain address.
		class HookNode {
		public:
			std::vector<HookHandle*> hooks = {};
			bool in_hook = false;
			size_t index = 0;
			void* return_address;
			uint8_t replaced_byte = 0;
			void* gateway = 0;
		};

		static inline std::map<const void*, HookNode> all_hooks = {};
		static inline bool initialized = false;

		static long NTAPI handler(EXCEPTION_POINTERS* info);
	};
}



#endif /* __LILAC_HOOK_HPP__ */