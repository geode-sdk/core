#ifndef GEODE_CORE_META_X86_HPP
#define GEODE_CORE_META_X86_HPP

#include <array>

namespace geode::core::meta::x86 {
    // Logic needed by x86 calling conventions for stack fixing / filtering.
    template <class Class>
    static constexpr bool sse_passable = 
        any_of<
            std::remove_cv_t<Class>,
            float, double
        >;

    template <class Class>
    static constexpr bool gpr_passable =
        (sizeof(Class) <= sizeof(void*) &&
        !std::is_class_v<Class> &&
        !sse_passable<Class>) ||
        std::is_member_function_pointer_v<Class>;


    template <class... Classes>
    static inline constexpr std::array<size_t, sizeof...(Classes)> reorder_structs() {
        constexpr size_t length = sizeof...(Classes);
        constexpr bool is_struct[] = {
            std::is_class_v<Classes>...
        };

        std::array<size_t, length> reordered = {};
        size_t out = 0;
        // The non-structs go first.
        for (size_t in = 0; in < length; ++in) {
            if (!is_struct[in]) {
                reordered[out] = in;
                ++out;
            }
        }

        // The structs go last.
        for (size_t in = 0; in < length; ++in) {
            if (is_struct[in]) {
                reordered[out] = in;
                ++out;
            }
        }
        
        return reordered;
    }
    
    template <class... Stack>
    static constexpr size_t stack_fix = 
        (((sizeof(Stack) % sizeof(void*) == 0) ?
            sizeof(Stack) :
            sizeof(Stack) - (sizeof(Stack) % sizeof(void*)) + sizeof(void*)) + ...);

    template <>
    static constexpr size_t stack_fix<> = 0;

    template <>
    static constexpr size_t stack_fix<void> = 0;

    template <class From>
    class Register {
    public:
        From raw;

    public:
        template <class To>
        explicit operator To() {
            static_assert(sizeof(From) >= sizeof(To), 
                "Please report a bug to the Geode developers! This should never be reached.\n"
                "Size of Register is smaller than the size of the destination type!");
            union {
                From from;
                To to;
            } u;
            u.from = raw;
            return u.to;
        }
    };
}

#endif /* GEODE_CORE_META_X86_HPP */