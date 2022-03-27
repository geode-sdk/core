#ifndef GEODE_CORE_META_MEMBERCALL_HPP
#define GEODE_CORE_META_MEMBERCALL_HPP

#include "tuple.hpp"
#include "x86.hpp"

#if 0
namespace geode::core::meta::x86 {
    template <class Ret, class... Args>
    class Membercall : public CallConv<Ret, Args...> {
    private:
        // Metaprogramming / typedefs we need for the rest of the class.
        using MyConv = CallConv<Ret, Args...>;
        // for some reason using definitions dont get inherited properly
        using MyTuple = typename MyConv::MyTuple;

    private:
        class Sequences {
        private:
            // These are required for proper reordering.
            static constexpr size_t length = sizeof...(Args);
            
            static constexpr size_t SSES = 3;
            static constexpr bool is_sse[length] = { sse_passable<Args>... };

            static constexpr bool is_gpr[length] = { gpr_passable<Args>... };
            
            static constexpr auto reordered_arr = reorder_structs<Args...>();

            // Setup call from our caller, to "foreign" function
            static constexpr auto filter_to() {
                /* The size of our output may be longer than the input.
                * For the third time, annoyingly, we need to make a lambda for this.
                */ 
                constexpr auto arr_size = []() -> size_t {
                    // Magic constant 5: XMM0, XMM4, XMM5, ECX, and EDX.
                    size_t size = length + SSES + 5;

                    // Check our only GPR.
                    if (0 < length && is_gpr[reordered_arr[0]]) {
                        --size;
                    }

                    /* We assume there are no SSES initially.
                    * Any SSES we encounter, we have to remove a "duplicate".
                    */
                    for (size_t i = 1; i < SSES + 1; ++i) {
                        if (i < length && is_sse[reordered_arr[i]]) {
                            --size;
                        }
                    }

                    return size;
                };
                std::array<size_t, arr_size()> to = {};

                // This is the index of the placeholder float and int, for clobbering SSEs and GPRs, respectively.
                constexpr size_t CLOBBER_SSE = length;
                constexpr size_t CLOBBER_GPR = length + 1;

                // Put the SSEs into the right XMM registers, if they exist.
                for (size_t i = 1; i < SSES + 1; ++i) {
                    if (i < length && is_sse[reordered_arr[i]]) {
                        to[i] = reordered_arr[i];
                    }
                    else {
                        to[i] = CLOBBER_SSE;
                    }
                }

                // Clobber XMM0, XMM4, XMM5, and EDX.
                to[0] = CLOBBER_SSE;
                to[4] = CLOBBER_SSE;
                to[5] = CLOBBER_SSE;
                to[7] = CLOBBER_GPR;

                // Handle our GPR and put it in ECX if we can.
                if (length > 0 && is_gpr[reordered_arr[0]]) {
                    to[6] = reordered_arr[0];
                }
                else {
                    to[6] = CLOBBER_GPR;
                }

                for (size_t in = 1, out = SSES + 5; in < length; ++in) {
                    // Put all non SSEs and non GPRs in their places.
                    size_t current = reordered_arr[in];
                    if (!(is_sse[current] && in < SSES + 1)) {
                        to[out] = current;
                        ++out;
                    }
                }

                return to;
            }

            // Setup call from "foreign" function, to one of ours.
            static constexpr auto filter_from() {
                std::array<size_t, length> from = {};

                constexpr size_t CLOBBER_SSE = length;
                constexpr size_t CLOBBER_GPR = length + 1;

                if (length > 0 && is_gpr[reordered_arr[0]]) {
                    from[reordered_arr[0]] = SSES + 3;
                }

                for (size_t i = 1, offset = 0; i < length; ++i) {
                    size_t current = reordered_arr[i];
                    if (is_sse[current])
                }

                return from;
            }

            // Grab only the stack values. For determining stack fixup.
            static constexpr auto filter_stack() {
                /* The size of our output may be shorter than the input.
                * For the fourth time, we need to make a lambda for this.
                */ 
                constexpr auto arr_size = []() -> size_t {
                    size_t size = 0;

                    return size;
                };
                std::array<size_t, arr_size()> stack = {};
            }

            // Annoyingly, unless we're using C++20, we can't eliminate these intermediates. (afaik)
            static constexpr auto to_arr = filter_to();
            static constexpr auto from_arr = filter_from();

        public:
            using to = arr_to_seq<to_arr>;
            using from = arr_to_seq<from_arr>;
        };
        // Filters that will be passed to Tuple::filter.
        template <size_t i, class Current, size_t c>
        class filter_to {
        public:
            static constexpr bool result = 
                (!gpr_passable<Current>::value || i != 0) &&
                (!sse_passable<Current>::value || i > 3);

            static constexpr size_t index = i;
            static constexpr size_t counter = c;
        };

        template <size_t i, class Current, size_t stack_offset>
        class filter_from {
        private:
            static constexpr bool sse = sse_passable<Current>::value && i <= 3 && i != 0;
            static constexpr bool gpr = gpr_passable<Current>::value && i == 0;

        public:
            // We're not even really filtering, just reordering.
            static constexpr bool result = true;

            static constexpr size_t index = 
                // If in GPR or SSE, retain index
                (gpr || sse) ? i
                // If on stack, offset by 4 (3 SSE + 1 GPR register(s) available)
                : stack_offset + 4;

            // If our output index is greater than 4, it has to be on stack. Increment.
            static constexpr size_t counter = stack_offset + static_cast<size_t>(index >= 4);
        };

    private:
        // Where all the logic is actually implemented. Needs to be instantiated by Optcall, though.
        template <class Class, class, class>
        class Impl {
            static_assert(always_false<Class>, 
                "Please report a bug to the Geode developers! This should never be reached.\n"
                "SFINAE didn't reach the right overload!");
        };

        template <size_t... to, size_t... from>
        class Impl<
            std::index_sequence<to...>, 
            std::index_sequence<from...>
        > {
        public:
            static Ret invoke(void* address, const Tuple<Args..., float, int>& all) {
                return reinterpret_cast<
                    Ret(__vectorcall*)(
                        typename Tuple<Args..., float, int>::template type_at<to>...
                    )
                >(address)(all.template at<to>()...);
            }

            template <Ret(* detour)(Args...)>
            static Ret __vectorcall wrapper(
                float,
                typename MyConv::template type_if<1, sse_passable, float> f1,
                typename MyConv::template type_if<2, sse_passable, float> f2,
                typename MyConv::template type_if<3, sse_passable, float> f3,
                float,
                float,
                typename MyConv::template type_if<0, gpr_passable, int> i0,
                int,
                // Not sure why this doesn't work otherwise, but oh well...
                typename MyConv::template type_at_wrap<to>... rest
            ) {
                auto all = Tuple<>::make(i0, f1, f2, f3, rest...);
                return detour(all.template at<from>()...);
            }
        };

    private:
        // Putting it all together: instantiating Impl with our filters.
        using MyImpl = 
            Impl<
                typename Sequences::to,
                typename Sequences::from
            >;

    public:
        // Just wrapping MyImpl.
        static Ret invoke(void* address, Args... all) {
            return MyImpl::invoke(address, { all..., 314.0f, 314 });
        }

        template <Ret(* detour)(Args...)>
        static constexpr decltype(auto) get_wrapper() {
            return &MyImpl::template wrapper<detour>;
        }
    };
}
#endif

#endif /* GEODE_CORE_META_MEMBERCALL_HPP */