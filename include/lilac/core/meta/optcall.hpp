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
                // We don't need to deal with rest here, since we will unconditionally pass them.
                auto all = Tuple<>::make(f0, f1, f2, f3, i0, i1);
                if constexpr (!std::is_same_v<Ret, void>) {
                    Ret ret = detour(all.template at<from>()..., rest...);
                    if constexpr (fix != 0) {
                        __asm sub esp, [fix]
                    }
                    return ret;
                }
                else {
                    detour(all.template at<from>()..., rest...);
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