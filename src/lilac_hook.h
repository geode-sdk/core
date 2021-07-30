#ifndef __LILAC_HOOK_H__
#define __LILAC_HOOK_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LILAC_CALL
	#undef LILAC_CALL
#endif
#define LILAC_CALL __vectorcall

typedef struct {
	const void* m_address;
	const void* m_detour;
} LilacHookHandle;

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
const LilacHookHandle* LILAC_CALL lilac_add_hook(const void* address, const void* detour);

/*
* params:
* handle - a pointer to the hook handle to be removed.
* 
* returns:
* true if the hook was successfully removed.
* false if removal failed.
*/
bool LILAC_CALL lilac_remove_hook(const LilacHookHandle* handle);

#ifdef __cplusplus
}
#endif

#endif /* __LILAC_HOOK_H__ */