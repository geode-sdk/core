#ifndef LILAC_CORE_HOOK_MACOS_HPP
#define LILAC_CORE_HOOK_MACOS_HPP

#include "platform.hpp"

namespace lilac::core::hook {
	class MacOSX : public Platform<MacOSX> {
	public:
		// wow these are the same
	#if defined(NDEBUG)
		static constexpr char trap[] = { '\xCC' };
	#else
		static constexpr char trap[] = { '\x0F', '\x0B' };
	#endif

	public:
		static bool write_memory(void* to, const void* from, size_t size);
		static bool initialize();
	};

	using TargetPlatform = Platform<MacOSX>;
}

#endif /* LILAC_CORE_HOOK_MACOS_HPP */
