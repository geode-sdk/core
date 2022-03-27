#ifndef GEODE_CORE_META_META_HPP
#define GEODE_CORE_META_META_HPP

#include "preproc.hpp"
#include "common.hpp"
#include "callconv.hpp"
#include "function.hpp"
#include "hook.hpp"

#if defined(GEODE_IS_WINDOWS)
    #include "cdecl.hpp"
    #include "optcall.hpp"
    #include "thiscall.hpp"
    #include "membercall.hpp"
#endif

#endif /* GEODE_CORE_META_META_HPP */