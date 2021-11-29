#ifndef LILAC_CORE_META_FUNCTION_HPP
#define LILAC_CORE_META_FUNCTION_HPP

#include "tuple.hpp"
#include "common.hpp"

#include <type_traits>

namespace lilac::core::meta {
    /* CRTP class for creating calling conventions for Function.
    * Provides some utilities for less verbose filtering of parameters.
    */
    template<class... Args>
    class CallConv {
    private:
        using MyTuple = Tuple<Args...>;

    protected:
        template<
            size_t i,
            template<class> class Pred,
            class Else
        >
        using type_if = 
            typename ternary<
                (MyTuple::size > i) &&
                Pred<typename MyTuple::template type_at<i>>::value
                >::template type<
                        typename MyTuple::template type_at<i>,
                        Else
                    >;

        template<
            size_t i,
            template<class> class Pred,
            class Else
        >
        static decltype(auto) value_if(const MyTuple& tuple, const Else e) {
            return ternary<
                    (MyTuple::size > i) &&
                    Pred<typename MyTuple::template type_at<i>>::value
                >::val(tuple.template at<i>(), e);
        }
    };

    /* The Lilac Function class wraps functions with unconventional
    *  calling conventions (how ironic).
    */
    template<
        class Func,
        template<class, class...> class Conv
    >
    class Function {
        static_assert(always_false<Func>, "Not a valid function pointer!");
    };

    template<
        class Ret, class... Args,
        template<class, class...> class Conv
    >
    class Function<Ret(Args...), Conv> {
    private:
        using MyConv = Conv<Ret, Args...>;
        using MyTuple = Tuple<Args...>;

    private:
        void* addr;

    public:
        template<class T>
        Function(const T& addr)
            : addr(reinterpret_cast<void*>(addr)) {}

        decltype(auto) operator()(Args... all) const {
            return MyConv::invoke(
                addr,
                { all... },
                typename MyTuple::template filter<MyConv::template filter>{}
            );
        }
    };
}

#endif /* LILAC_CORE_META_FUNCTION_HPP */