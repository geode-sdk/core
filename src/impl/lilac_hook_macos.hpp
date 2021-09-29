#ifndef __LILAC_HOOK_MACOS_HPP__
#define __LILAC_HOOK_MACOS_HPP__

#include "lilac_hook_platform.hpp"

namespace lilac::impl {
	class MacOSX : public Platform<MacOSX> {
	public:
		// wow these are the same
	#if defined(NDEBUG)
		static constexpr char trap[] = { '\xCC' };
	#else
		static constexpr char trap[] = { '\x0F', '\x0B' };
	#endif

	public:
		static void write_memory(void* to, const void* from, size_t size);
		static bool initialize();
	};

	using TargetPlatform = Platform<MacOSX>;


}

#endif /* __LILAC_HOOK_MACOS_HPP__ */
