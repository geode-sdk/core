#ifndef LILAC_CORE_META_PREPROC_HPP
#define LILAC_CORE_META_PREPROC_HPP

#if defined(LILAC_CALL)
    #undef LILAC_CALL
#endif

#if defined(_WIN32)
    #define LILAC_TARGET_WINDOWS
    #define LILAC_CALL __stdcall
#elif defined(__APPLE__)
    #include <TargetConditionals.h>
    #if TARGET_OS_MAC
        #define LILAC_TARGET_MACOS
    #elif TARGET_OS_IPHONE
        #define LILAC_TARGET_IOS
    #endif
    #define LILAC_CALL
#else
    #error "Currently unsupported platform."
#endif

namespace lilac::core::meta {
    #if defined(NDEBUG)
        static constexpr bool debug = false;
    #else
        static constexpr bool debug = true;
    #endif
}


#endif /* LILAC_CORE_META_PREPROC_HPP */