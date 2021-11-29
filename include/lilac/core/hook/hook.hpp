#ifndef LILAC_CORE_HOOK_HPP
#define LILAC_CORE_HOOK_HPP

#include "../meta/preproc.hpp"

namespace lilac::core::hook {
	/* opaque struct representing a handle to a hook.
	*/
	using Handle = void*;

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
	Handle LILAC_CALL add(const void* address, const void* detour);

	/*
	* params:
	* handle - a pointer to the hook handle to be removed.
	*
	* returns:
	* true if the hook was successfully removed.
	* false if removal failed.
	*/
	bool LILAC_CALL remove(Handle handle);
}

#endif /* LILAC_CORE_HOOK_HPP */
