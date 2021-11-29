#ifndef LILAC_CORE_HOOK_WINDOWS_HPP
#define LILAC_CORE_HOOK_WINDOWS_HPP

#include "platform.hpp"

namespace lilac::core::hook {
	class WindowsX86 : public Platform<WindowsX86> {
	public:
	#if defined(NDEBUG)
		static constexpr char trap[] = { '\xCC' };
	#else
		static constexpr char trap[] = { '\x0F', '\x0B' };
	#endif

	public:
		static void write_memory(void* to, const void* from, size_t size);
		static bool initialize();
	};

	using TargetPlatform = Platform<WindowsX86>;
}

#endif /* LILAC_CORE_HOOK_WINDOWS_HPP */