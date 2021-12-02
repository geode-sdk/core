#ifndef LILAC_CORE_META_PREPROC_HPP
#define LILAC_CORE_META_PREPROC_HPP

// for some reason cmake just doesn't want to 
// include the lilac_utils subdirectories 
// so i have to do this
#include "../../../../utils/include/lilac/Platform.hpp"

namespace lilac::core::meta {
    #if defined(NDEBUG)
        static constexpr bool debug = false;
    #else
        static constexpr bool debug = true;
    #endif
}

#endif /* LILAC_CORE_META_PREPROC_HPP */
