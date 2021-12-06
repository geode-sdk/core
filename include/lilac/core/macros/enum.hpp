#pragma once

#include "foreach.hpp"

#define FIRST_ARG_(N, ...) N
#define FIRST_ARG(args) FIRST_ARG_ args

#define FIRST_TYPE(...) decltype(FIRST_ARG((__VA_ARGS__)))
#define SWITCH_CASE(x) case x: return #x;

#define ENUMERATIVE(...)                                                \
    enum { __VA_ARGS__ };                                               \
    template<class T>                                                   \
    static FIRST_TYPE(__VA_ARGS__) cast(T t) {                          \
        return static_cast<FIRST_TYPE(__VA_ARGS__)>(t);                 \
    }                                                                   \
    static constexpr const char* toString(FIRST_TYPE(__VA_ARGS__) t) {  \
        switch (t) { FOR_EACH(SWITCH_CASE, , __VA_ARGS__) }             \
        return "Undefined";                                             \
    }                                                                   \
