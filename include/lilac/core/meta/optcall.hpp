#ifndef LILAC_CORE_META_OPTCALL_HPP
#define LILAC_CORE_META_OPTCALL_HPP

#include "function.hpp"
#include "hook.hpp"
#include "tuple.hpp"
#include "x86.hpp"

namespace lilac::core::meta::x86 {
    template<class Ret, class... Args>
    class Optcall : public CallConv<Optcall, Ret, Args...> {
    private:
        // Metaprogramming / typedefs we need for the rest of the class.
        class FilterOut {};

        using MyConv = CallConv<Optcall, Ret, Args...>;
        using MyTuple = typename MyConv::MyTuple;
        using MyTupleFrom = 
            Tuple<
                typename MyConv::template type_if<0, sse_passable, FilterOut>,
                typename MyConv::template type_if<1, sse_passable, FilterOut>,
                typename MyConv::template type_if<2, sse_passable, FilterOut>,
                typename MyConv::template type_if<3, sse_passable, FilterOut>,
                typename MyConv::template type_if<0, gpr_passable, FilterOut>,
                typename MyConv::template type_if<1, gpr_passable, FilterOut>
            >;

    private:
        // Filters that will be passed to Tuple::filter.
        template<size_t i, class Current>
        struct filter_to {
            static constexpr bool value = 
                (!gpr_passable<Current>::value || i > 1) &&
                (!sse_passable<Current>::value || i > 3);
        };

        template<size_t i, class Current>
        struct filter_from {
            static constexpr bool value = !std::is_same_v<Current, FilterOut>;
        };

    private:
        // Where all the logic is actually implemented. Needs to be instantiated by Optcall, though.
        template<class Class, class>
        class Impl {
            static_assert(always_false<Class>, 
                "Please report a bug to the Lilac developers! This should never be reached.");
        };

        template<size_t... to, size_t... from>
        class Impl<std::index_sequence<to...>, std::index_sequence<from...>> {
        private:
            static constexpr size_t fix =
                (std::is_class_v<Ret> ? stack_fix<Ret> : 0)
                + stack_fix<typename MyTuple::template type_at<to>...>;

        public:
            static Ret invoke(void* address, const Tuple<Args...>& all) {
                Ret(__vectorcall* raw)(
                    typename MyConv::template type_if<0, sse_passable, float>,
                    typename MyConv::template type_if<1, sse_passable, float>,
                    typename MyConv::template type_if<2, sse_passable, float>,
                    typename MyConv::template type_if<3, sse_passable, float>,
                    float,
                    float,
                    typename MyConv::template type_if<0, gpr_passable, int>,
                    typename MyConv::template type_if<1, gpr_passable, int>,
                    typename MyTuple::template type_at<to>...
                ) = reinterpret_cast<decltype(raw)>(address);

                if constexpr (!std::is_same_v<Ret, void>) {
                    Ret ret = raw(
                        MyConv::template value_if<0, sse_passable>(all, 1907.0f),
                        MyConv::template value_if<1, sse_passable>(all, 1907.0f),
                        MyConv::template value_if<2, sse_passable>(all, 1907.0f),
                        MyConv::template value_if<3, sse_passable>(all, 1907.0f),
                        1907.0f,
                        1907.0f,
                        MyConv::template value_if<0, gpr_passable>(all, 1907),
                        MyConv::template value_if<1, gpr_passable>(all, 1907),
                        all.template at<to>()...
                    );

                    if constexpr (fix != 0) {
                        __asm add esp, [fix]
                    }

                    return ret;
                }
                else {
                    raw(
                        MyConv::template value_if<0, sse_passable>(all, 1907.0f),
                        MyConv::template value_if<1, sse_passable>(all, 1907.0f),
                        MyConv::template value_if<2, sse_passable>(all, 1907.0f),
                        MyConv::template value_if<3, sse_passable>(all, 1907.0f),
                        1907.0f,
                        1907.0f,
                        MyConv::template value_if<0, gpr_passable>(all, 1907),
                        MyConv::template value_if<1, gpr_passable>(all, 1907),
                        all.template at<to>()...
                    );

                    if constexpr (fix != 0) {
                        __asm add esp, [fix]
                    }
                }
            }

            template <size_t index, size_t stack_offset, size_t... seq>
            static constexpr auto gen_call_indices(std::index_sequence<seq...> ind) {
                using type = typename MyTuple::template type_at<index>;
                constexpr bool is_sse = sse_passable<type>::value;
                constexpr bool is_gpr = gpr_passable<type>::value;
                if constexpr (index == MyTuple::size) return ind;
                else {
                    constexpr auto stack_index = 6 + stack_offset;
                    constexpr size_t target_index = 
                        // can be xmm0, xmm1, ecx, edx or stack
                        index <= 1 ? (is_sse ? index : is_gpr ? index + 4 : stack_index) :
                        // can be either xmm2, xmm3 or stack
                        index <= 3 ? (is_sse ? index : stack_index) :
                        // is on stack
                        stack_index + index - 4;
                    constexpr auto next_offset = stack_offset + (index < 4 && !is_sse && (index > 1 || !is_gpr) ? 1 : 0);
                    return gen_call_indices<index + 1, next_offset>(std::index_sequence<seq..., target_index>{});
                }
            }

            using CallIndices = decltype(gen_call_indices<0, 0>(std::index_sequence<>{}));

            // this is a terrible name, should this even be here
            template <auto func, class T, size_t... seq>
            static decltype(auto) apply_func_with_indices(T&& tuple, std::index_sequence<seq...>) {
                return func(tuple.template at<seq>()...);
            }

            template <Ret(* detour)(Args...)>
            static Ret __vectorcall wrapper_impl(
                typename MyConv::template type_if<0, sse_passable, float> f0,
                typename MyConv::template type_if<1, sse_passable, float> f1,
                typename MyConv::template type_if<2, sse_passable, float> f2,
                typename MyConv::template type_if<3, sse_passable, float> f3,
                float,
                float,
                typename MyConv::template type_if<0, gpr_passable, int> i0,
                typename MyConv::template type_if<1, gpr_passable, int> i1,
                typename MyTuple::template type_at<to>... rest
            ) {
                auto all = Tuple<>::make(f0, f1, f2, f3, i0, i1, rest...);
                if constexpr (!std::is_same_v<Ret, void>) {
                    Ret ret = apply_func_with_indices<detour>(all, CallIndices{});
                    // TODO: these stack fixes are sus
                    if constexpr (fix != 0) {
                        __asm sub esp, [fix]
                    }
                    return ret;
                } else {
                    apply_func_with_indices<detour>(all, CallIndices{});
                    if constexpr (fix != 0) {
                        __asm sub esp, [fix]
                    }
                }
            }

            template <Ret(* detour)(Args...)>
            static decltype(auto) get_wrapper() {
                return &wrapper_impl<detour>;
            }
        };

    private:
        // Putting it all together: instantiating Impl with our filters.
        using MyImpl = 
            Impl<
                typename MyConv::MyTuple::template filter<filter_to>,
                typename MyTupleFrom::template filter<filter_from>
            >;

    public:
        // Just wrapping MyImpl.
        static Ret invoke(void* address, const Tuple<Args...>& all) {
            return MyImpl::invoke(address, all);
        }

        template<Ret(* detour)(Args...)>
        static decltype(auto) get_wrapper() {
            return MyImpl::template get_wrapper<detour>();
        }
    };
}

#endif /* LILAC_CORE_META_OPTCALL_HPP */