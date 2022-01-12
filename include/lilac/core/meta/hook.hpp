#ifndef LILAC_CORE_META_HOOK_HPP
#define LILAC_CORE_META_HOOK_HPP

#include "tuple.hpp"
#include "common.hpp"
#include "callconv.hpp"

#include "../hook/hook.hpp"

#include <type_traits>

namespace lilac::core::meta {
    template<
        template<class, class...> class Conv,
        auto address,
        auto detour
    >
    class Hook {
        static_assert(always_false<decltype(address)>, 
            "Not a valid function pointer, or hook and detour aren't compatible!");
    };
    
    template<
        template<class, class...> class Conv,
        class Ret,
        class... Args,
        Ret(* address)(Args...),
        Ret(* detour)(Args...)
    >
    class Hook<Conv, address, detour> {
    private:
        using MyConv = Conv<Ret, Args...>;
        using MyTuple = Tuple<Args...>;

        /* TODO: REMOVE THIS!!!
        template<class>
        class Wrapper;

        template<class... RawArgs>
        class Wrapper<Tuple<RawArgs...>> {
        private:
            using MyTuple = Tuple<RawArgs...>;

            template<size_t... indices>
            decltype(auto) get_impl(const std::index_sequence<indices...>&&) {
                return [](RawArgs... raw_args) -> Ret {
                    const MyTuple tuple = { raw_args... };
                    return detour(tuple.template at<indices>()...);
                };
            }

        public:
            decltype(auto) get() {
                return get_impl(
                    typename MyTuple::template filter<MyConv::template filter>{}
                );
            }
        };
        */
    private:
        static inline lilac::core::hook::Handle handle;

    public:
        Hook() {
            auto wrapper = MyConv::get_wrapper<detour>(
                typename MyTuple::template filter<MyConv::template filter> {}
            );
            this->handle = lilac::core::hook::add(address, wrapper);
        }
    };

    // member functions.
    template<
        template<class, class...> class Conv,
        class Ret, class Parent, class... Args,
        Ret(Parent::* address)(Args...),
        Ret(Parent::* detour)(Args...)
    >
    class Hook<Conv, address, detour> {
        // deal with this later lol
    };
}


#endif /* LILAC_CORE_META_HOOK_HPP */