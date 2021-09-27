#include "lilac_hook_macos.hpp"
#include "lilac_hook_impl.hpp"

#include <mach/mach_vm.h>       /* mach_vm_*            */
#include <mach/mach_init.h>     /* mach_task_self()     */
#include <signal.h>             /* sigaction            */
#include <sys/ucontext.h>
#include <iostream>

using namespace lilac;
using namespace impl;

namespace {
    void handler(int signal, siginfo_t* signal_info, void* vcontext) {
        // std::cout << "idfk" << std::endl;
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

void MacOSX::write_memory(void* to, const void* from, size_t size) {

    std::cout << "to " << to << " from " << from << " size " << size << std::endl;

    
    // kern_return_t ret = mach_vm_protect(mach_task_self(), (mach_vm_address_t)to, size, FALSE, VM_PROT_WRITE | VM_PROT_READ | VM_PROT_EXECUTE);
    kern_return_t ret = mach_vm_protect(mach_task_self(), (mach_vm_address_t)to, size, FALSE, VM_PROT_EXECUTE);
    std::cout << ret << std::endl;
    // if (ret != KERN_SUCCESS) return;

    // ret = mach_vm_write(mach_task_self(), (mach_vm_address_t)to, (vm_offset_t)from, (mach_msg_type_number_t)size);
    // std::cout << ret << std::endl;
    // if (ret != KERN_SUCCESS) return;

    // std::cout << "success" << std::endl;

    std::memcpy(to, from, size);
}

bool MacOSX::initialize() {
    std::cout << "a" << std::endl;
    std::cout << "uwu " << (void*)&(MacOSX::write_memory) << std::endl;
    int a;
    std::cin >> a;
    struct sigaction action;
    memset(&action, '\0', sizeof(action));
    action.sa_sigaction = &handler;
    action.sa_flags = SA_SIGINFO;

    #if defined(NDEBUG)
        int signal = SIGTRAP;
    #else
        int signal = SIGILL;
    #endif

    // std::cout << SIGILL << "  " << signal << std::endl;
    
    return sigaction(signal, &action, NULL) < 0;
}
