#ifndef LILAC_CORE_META_HOOK_HPP
#define LILAC_CORE_META_HOOK_HPP

#include "tuple.hpp"
#include "common.hpp"

#include <type_traits>

namespace lilac::core::meta {
    template<
        template<class, class...> class Conv,
        auto address
    >
    class Hook {
        static_assert(always_false<decltype(address)>, "Not a valid function pointer!");
    };

    template<
        template<class, class...> class Conv,
        class Ret,
        class... Args,
        Ret(*address)(Args...)
    >
    class Hook<Conv, address> {
    private:
        using MyConv = Conv<Ret, Args...>;
        using MyTuple = Tuple<Args...>;

    private:
        std::conditional_t<
            std::is_member_function_pointer_v<decltype(address)>,
            decltype(address),
            Ret(*)(Args...)
        >
         addr;

    public:
        Hook(Ret(*addr)(Args...))
            : addr(addr) {
            static_assert(
                std::is_same_v<
                    std::remove_cv_t<decltype(addr)>,
                    std::remove_cv_t<Ret(*)(Args...)>
                >,
                "Hook and detour are not compatible!"
            );
            // We need to invoke lilac_add_hook here
        }
    };

    template<
        template<class, class...> class Conv,
        auto address,
        class Ret, class Parent, class... Args
    >
    class Hook<Conv, address, Ret(Parent::*)(Args...)> {

    };
}


#endif /* LILAC_CORE_META_HOOK_HPP */