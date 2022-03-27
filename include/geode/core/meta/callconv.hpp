#ifndef GEODE_CORE_META_CALLCONV_HPP
#define GEODE_CORE_META_CALLCONV_HPP

#include <array>
#include <utility>

namespace geode::core::meta {
    /* CRTP class for creating calling conventions for Function and Hook.
    * Provides some utilities for less verbose filtering of parameters, and
    * some wrappers to require the custom calling convention to supply an
    * invoker and a way to get a wrapper for hooks.
    */
    template <class Ret, class... Args>
    class CallConv {
    protected:
        using MyTuple = Tuple<Args...>;

        template <bool, size_t i>
        class type_at_wrap_impl {
        public:
            using result = void;
        };

        template <size_t i>
        class type_at_wrap_impl<true, i> {
        public:
            using result = typename MyTuple::template type_at<i>;
        };

        template <size_t i>
        using type_at_wrap = typename type_at_wrap_impl<(i < MyTuple::size), i>::result;

        template <size_t i>
        static constexpr decltype(auto) at_wrap(const MyTuple& tuple) {
            if constexpr (i < MyTuple::size) {
                return tuple.template at<i>();
            }
            else {
                return 0;
            }
        }

        template <
            size_t i,
            template <class> class Pred,
            class Else
        >
        using type_if = 
            typename ternary<
                    (MyTuple::size > i) &&
                    Pred<type_at_wrap<i>>::value
                >::template type<
                        type_at_wrap<i>,
                        Else
                    >;

        template <
            size_t i,
            template <class> class Pred,
            class Else
        >
        static constexpr decltype(auto) value_if(const MyTuple& tuple, const Else e) {
            return ternary<
                    (MyTuple::size > i) &&
                    Pred<type_at_wrap<i>>::value
                >::val(at_wrap<i>(tuple), e);
        }

        template <auto>
        class arr_to_seq_impl;

        template <class Type, size_t length, const std::array<Type, length>* arr>
        class arr_to_seq_impl<arr> {
        private:
            template <class>
            class getter;

            template <size_t... indices>
            class getter<std::index_sequence<indices...>> {
            public:
                using result = std::index_sequence<arr->at(indices)...>;
            };
            
        public:
            using result = typename getter<std::make_index_sequence<length>>::result;
        };

        template <auto& arr>
        using arr_to_seq = typename arr_to_seq_impl<&arr>::result;
    };
}

#endif /* GEODE_CORE_META_CALLCONV_HPP */
