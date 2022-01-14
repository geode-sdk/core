#ifndef LILAC_CORE_META_OPTCALL_HPP
#define LILAC_CORE_META_OPTCALL_HPP

#include "function.hpp"
#include "hook.hpp"
#include "tuple.hpp"

namespace lilac::core::meta::x86 {
    template<class Ret, class... Args>
    class Optcall : public CallConv<Optcall, Ret, Args...> {
    private:
        using MyConv = CallConv<Optcall, Ret, Args...>;

        template<class Class>
        struct gpr_passable {
            static constexpr bool value = 
                any_of<
                    std::remove_cv_t<Class>,
                    bool, signed char, signed short, signed int, signed long,
                    unsigned char, unsigned short, unsigned int, unsigned long,
                    char, short, int, long
                > || 
                std::is_pointer_v<Class> || 
                std::is_reference_v<Class>;
        };

        template<class Class>
        struct sse_passable {
            static constexpr bool value = 
                any_of<
                    std::remove_cv_t<Class>,
                    float, double
                >;
        };

        template<class... Stack>
        static constexpr size_t stack_fix = 
            (((sizeof(Stack) % sizeof(void*) == 0) ?
                sizeof(Stack) :
                sizeof(Stack) - (sizeof(Stack) % sizeof(void*)) + sizeof(void*)) + ...);

        template<>
        static constexpr size_t stack_fix<> = 0;

        static constexpr size_t ret_fix =
            std::is_class_v<Ret> ?
                stack_fix<Ret> : 0;

    public:
        template<size_t i, class Current>
        struct filter {
            static constexpr bool value = 
                (!gpr_passable<Current>::value || i > 1) &&
                (!sse_passable<Current>::value || i > 3);
        };

        template<size_t... indices>
        static Ret invoke(
            Ret(* address)(Args...),
            const Tuple<Args...>&& all,
        ) {
            Ret(__vectorcall* raw)(
                typename MyConv::template type_if<0, sse_passable, float>,
                typename MyConv::template type_if<1, sse_passable, float>,
                typename MyConv::template type_if<2, sse_passable, float>,
                typename MyConv::template type_if<3, sse_passable, float>,
                float,
                float,
                typename MyConv::template type_if<0, gpr_passable, int>,
                typename MyConv::template type_if<1, gpr_passable, int>,
                typename MyTuple::template type_at<indices>...
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
                    all.template at<indices>()...
                );

                static constexpr size_t fix = ret_fix + stack_fix<typename MyTuple::template type_at<indices>...>;
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
                    all.template at<indices>()...
                );

                static constexpr size_t fix = ret_fix + stack_fix<typename MyTuple::template type_at<indices>...>;
                if constexpr (fix != 0) {
                    __asm add esp, [fix]
                }
            }
        }

        template<Ret(* detour)(Args...), size_t indices...>
        static decltype(auto) get_wrapper() {
            Ret(__vectorcall* wrapper)(
                typename MyConv::template type_if<0, sse_passable, float>,
                typename MyConv::template type_if<1, sse_passable, float>,
                typename MyConv::template type_if<2, sse_passable, float>,
                typename MyConv::template type_if<3, sse_passable, float>,
                float,
                float,
                typename MyConv::template type_if<0, gpr_passable, int>,
                typename MyConv::template type_if<1, gpr_passable, int>,
                typename MyTuple::template type_at<indices>...
            ) = 
                // Cacao be like:
                [](
                    auto f0, auto f1, auto f2, auto f3,
                    float, float,
                    auto i0, auto i1,
                    typename MyTuple::template type_at<indices>... rest
                ) -> Ret {
                    /* Tuple<
                        decltype(f0), decltype(f1), decltype(f2), decltype(f3),
                        float, float,
                        decltype(i0), decltype(i1), 
                        decltype(rest)...
                    > all = { f0, f1, f2, f3, 1907.0f, 1907.0f, i0, i1, rest...}; */

                };
        }
    };
    }
}

#endif /* LILAC_CORE_META_OPTCALL_HPP */