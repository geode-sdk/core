#ifndef GEODE_CORE_META_FUNCTION_HPP
#define GEODE_CORE_META_FUNCTION_HPP

#include "callconv.hpp"
#include "common.hpp"
#include "tuple.hpp"

#include <type_traits>

namespace geode::core::meta {
    /* The Geode Function class wraps functions with unconventional
     *  calling conventions (how ironic).
     */
    template <class Func, template <class, class...> class Conv>
    class Function {
        static_assert(always_false<Func>, "Not a valid function pointer!");
    };

    template <class Ret, class... Args, template <class, class...> class Conv>
    class Function<Ret(Args...), Conv> {
    private:
        using MyConv = Conv<Ret, Args...>;

    private:
        void* addr;

    public:
        template <class Pointer>
        Function(const Pointer& addr) : addr(reinterpret_cast<void*>(addr)) {}

        decltype(auto) operator()(Args... all) const {
            return MyConv::invoke(addr, all...);
        }
    };
}

#endif /* GEODE_CORE_META_FUNCTION_HPP */