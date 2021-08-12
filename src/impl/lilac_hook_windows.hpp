#ifndef __LILAC_HOOK_WINDOWS_HPP__
#define __LILAC_HOOK_WINDOWS_HPP__

#include "lilac_hook_platform.hpp"

#define PLATFORM lilac::impl::WindowsX86

namespace lilac::impl {
	class WindowsX86 : public Platform<WindowsX86> {
	public:
	#ifdef NDEBUG
		static constexpr char trap[] = { '\xCC' };
	#else
		static constexpr char trap[] = { '\x0F', '\x0B' };
	#endif

	public:
		static void write_memory(void* to, const void* from, size_t size);
		static bool initialize();
	};
}

#endif /* __LILAC_HOOK_WINDOWS_HPP__ */