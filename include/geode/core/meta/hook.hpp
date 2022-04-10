#ifndef GEODE_CORE_META_HOOK_HPP
#define GEODE_CORE_META_HOOK_HPP

#include "../hook/hook.hpp"
#include "callconv.hpp"
#include "common.hpp"
#include "tuple.hpp"

#include <type_traits>

namespace geode::core::meta {
    template <auto address, auto detour, template <class, class...> class Conv>
    class Hook {
        static_assert(
            always_false<decltype(address)>,
            "Not a valid function pointer, or hook and detour aren't compatible!"
        );
    };

    template <
        class Ret, class... Args,
        // TODO: Fix this!!! I hate type qualifiers.
        auto address, Ret (*detour)(Args...), template <class, class...> class Conv>
    class Hook<address, detour, Conv> {
    private:
        using MyConv = Conv<Ret, Args...>;
        using MyTuple = Tuple<Args...>;

    private:
        static inline geode::core::hook::Handle handle;

    public:
        Hook() {
            auto wrapper = MyConv::template get_wrapper<detour>();
            this->handle = geode::core::hook::add(
                reinterpret_cast<void*>(address), reinterpret_cast<void*>(wrapper)
            );
        }
    };

    // Member functions.
    template <
        class Ret, class Parent, class... Args, Ret (Parent::*address)(Args...),
        Ret (Parent::*detour)(Args...), template <class, class...> class Conv>
    class Hook<address, detour, Conv> {
        // Deal with this later lol
    };
}

#endif /* GEODE_CORE_META_HOOK_HPP */