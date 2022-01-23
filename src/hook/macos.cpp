#include "macos.hpp"
#include "impl.hpp"

#include <mach/mach_vm.h>       /* mach_vm_*            */
#include <mach/mach_init.h>     /* mach_task_self()     */

#include <signal.h>             /* sigaction            */
#include <sys/ucontext.h>       /* ucontext_t           */

using namespace geode::core::hook;

namespace {
    void handler(int signal, siginfo_t* signal_info, void* vcontext) {
        ucontext_t* context = reinterpret_cast<ucontext_t*>(vcontext);

        const void* ret = *reinterpret_cast<void**>(context->uc_mcontext->__ss.__rsp);
        const void** current = reinterpret_cast<const void**>(&context->uc_mcontext->__ss.__rip);

        Exception exception = {
            signal_info->si_addr,
            ret,
            *current
        };

        HookManager::handler(exception);
    }
}

bool MacOSX::write_memory(void* to, const void* from, size_t size) {
    kern_return_t ret;
    
    ret = mach_vm_protect(mach_task_self(), (mach_vm_address_t)to, size, FALSE, VM_PROT_COPY | VM_PROT_EXECUTE | VM_PROT_WRITE | VM_PROT_READ);
    if (ret != KERN_SUCCESS) return false;

    ret = mach_vm_write(mach_task_self(), (mach_vm_address_t)to, (vm_offset_t)from, (mach_msg_type_number_t)size);

    return ret == KERN_SUCCESS;
}

bool MacOSX::initialize() {
    struct sigaction action;
    memset(&action, '\0', sizeof(action));
    action.sa_sigaction = &handler;
    action.sa_flags = SA_SIGINFO;

    #if defined(NDEBUG)
        int signal = SIGTRAP;
    #else
        int signal = SIGILL;
    #endif

    return sigaction(signal, &action, NULL) < 0;
}
