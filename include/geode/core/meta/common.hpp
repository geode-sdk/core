#ifndef GEODE_CORE_META_COMMON_HPP
#define GEODE_CORE_META_COMMON_HPP

#include <type_traits>

namespace geode::core::meta {
    template <bool cond>
    class ternary {
    public:
        template <class T, class F>
        static constexpr decltype(auto) val(
            const T t,
            const F f
        ) { return f; }

        template <class T, class F>
        using type = F;
    };

    template <>
    class ternary<true> {
    public:
        template <class T, class F>
        static constexpr decltype(auto) val(
            const T t,
            const F f
        ) { return t; }

        template <class T, class F>
        using type = T;
    };

    template <class Type, class... Compare>
    static constexpr bool any_of = (std::is_same_v<Type, Compare> || ...);

    template <class... Classes>
    static constexpr bool always_false = false;
}

#endif /* GEODE_CORE_META_COMMON_HPP */