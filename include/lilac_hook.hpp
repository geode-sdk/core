#ifndef __LILAC_HOOK_HPP__
#define __LILAC_HOOK_HPP__

#include "lilac_hook.h"

namespace lilac {
	using HookHandle = LilacHookHandle;

	class Hooks {
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
		* the hooked function should have the same calling convention and parameters
		* as the detour. otherwise, crashing is almost certain to occur.
		*/
		static HookHandle LILAC_CALL add(const void* address, const void* detour);

		/*
		* params:
		* handle - a pointer to the hook handle to be removed.
		*
		* returns:
		* true if the hook was successfully removed.
		* false if removal failed.
		*/
		static bool LILAC_CALL remove(HookHandle handle);
	};
}

#endif /* __LILAC_HOOK_HPP__ */
