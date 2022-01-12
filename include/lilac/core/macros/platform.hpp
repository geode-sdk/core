#pragma once

#if defined(LILAC_CALL)
	#undef LILAC_CALL
#endif

#include "enum.hpp"

namespace lilac {
	class PlatformID {
	public:
		enum {
			Unknown = -1,
			Windows,
			MacOS,
			iOS,
			Android,
			Linux,
		};
		
		using Type = decltype(Unknown);

		Type m_value;

		PlatformID(Type t) { m_value = t; }
		PlatformID& operator=(Type t) { m_value = t; }
		bool operator==(int other) const { return m_value == other; }
		bool operator==(PlatformID const& other) const { return m_value == other.m_value; }
		operator int() { return m_value; }

		template<class T>
		static Type cast(T t) {
			return static_cast<Type>(t);
		}

		static constexpr const char* toString(Type lp) {
			switch (lp) {
				case Unknown: return "Unknown";
				case Windows: return "Windows";
				case MacOS:   return "MacOS";
				case iOS:     return "iOS";
				case Android: return "Android";
				case Linux:   return "Linux";
			}
			return "Undefined";
		}
	};
}

// Windows
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#define LILAC_WINDOWS(...) __VA_ARGS__
	#define LILAC_IS_WINDOWS
	#define LILAC_IS_DESKTOP
	#define LILAC_PLATFORM_NAME "Windows"
	#define LILAC_PLATFORM_TARGET PlatformID::Windows
	#define LILAC_CALL __stdcall
#else
	#define LILAC_WINDOWS(...)
#endif

// MacOS / iOS
#if defined(__APPLE__)
	#include <TargetConditionals.h>
	#if TARGET_OS_MAC
		#define LILAC_IOS(...)
		#define LILAC_MACOS(...) __VA_ARGS__
		#define LILAC_IS_MACOS
		#define LILAC_IS_DESKTOP
		#define LILAC_PLATFORM_NAME "MacOS"
		#define LILAC_PLATFORM_TARGET PlatformID::MacOS
	#elif TARGET_OS_IPHONE
		#define LILAC_MACOS(...)
		#define LILAC_IOS(...) __VA_ARGS__
		#define LILAC_IS_IOS
		#define LILAC_IS_MOBILE
		#define LILAC_PLATFORM_NAME "iOS"
		#define LILAC_PLATFORM_TARGET PlatformID::iOS
	#endif
	#define LILAC_CALL
#else
	#define LILAC_MACOS(...)
	#define LILAC_IOS(...)
#endif

// Android
#if defined(__ANDROID__)
	#define LILAC_ANDROID(...) __VA_ARGS__
	#define LILAC_IS_ANDROID
	#define LILAC_IS_MOBILE
	#define LILAC_PLATFORM_NAME "Android"
	#define LILAC_PLATFORM_TARGET PlatformID::Android
	#define LILAC_CALL
#else
	#define LILAC_ANDROID(...)
#endif

#ifndef LILAC_PLATFORM_NAME
	#error "Unsupported PlatformID!"
#endif
