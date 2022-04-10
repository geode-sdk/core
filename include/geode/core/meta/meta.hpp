#ifndef GEODE_CORE_META_META_HPP
#define GEODE_CORE_META_META_HPP

#include "callconv.hpp"
#include "common.hpp"
#include "function.hpp"
#include "hook.hpp"
#include "preproc.hpp"

#if defined(GEODE_IS_WINDOWS)
    #include "cdecl.hpp"
    #include "membercall.hpp"
    #include "optcall.hpp"
    #include "thiscall.hpp"
#endif

#endif /* GEODE_CORE_META_META_HPP */