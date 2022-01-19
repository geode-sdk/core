#pragma once

#if defined(GEODE_CALL)
	#undef GEODE_CALL
#endif

#include "enum.hpp"

namespace geode {
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
		PlatformID& operator=(Type t) { m_value = t; return *this; }
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
	#define GEODE_WINDOWS(...) __VA_ARGS__
	#define GEODE_IS_WINDOWS
	#define GEODE_IS_DESKTOP
	#define GEODE_PLATFORM_NAME "Windows"
	#define GEODE_PLATFORM_TARGET PlatformID::Windows
	#define GEODE_CALL __stdcall
	#define GEODE_PLATFORM_EXTENSION ".dll"
#else
	#define GEODE_WINDOWS(...)
#endif

// MacOS / iOS
#if defined(__APPLE__)
	#include <TargetConditionals.h>
	#if TARGET_OS_MAC
		#define GEODE_IOS(...)
		#define GEODE_MACOS(...) __VA_ARGS__
		#define GEODE_IS_MACOS
		#define GEODE_IS_DESKTOP
		#define GEODE_PLATFORM_NAME "MacOS"
		#define GEODE_PLATFORM_TARGET PlatformID::MacOS
		#define GEODE_PLATFORM_EXTENSION ".dylib"
	#elif TARGET_OS_IPHONE
		#define GEODE_MACOS(...)
		#define GEODE_IOS(...) __VA_ARGS__
		#define GEODE_IS_IOS
		#define GEODE_IS_MOBILE
		#define GEODE_PLATFORM_NAME "iOS"
		#define GEODE_PLATFORM_TARGET PlatformID::iOS
		#define GEODE_PLATFORM_EXTENSION ".dylib"
	#endif
	#define GEODE_CALL
#else
	#define GEODE_MACOS(...)
	#define GEODE_IOS(...)
#endif

// Android
#if defined(__ANDROID__)
	#define GEODE_ANDROID(...) __VA_ARGS__
	#define GEODE_IS_ANDROID
	#define GEODE_IS_MOBILE
	#define GEODE_PLATFORM_NAME "Android"
	#define GEODE_PLATFORM_TARGET PlatformID::Android
	#define GEODE_CALL
	#define GEODE_PLATFORM_EXTENSION ".so"
#else
	#define GEODE_ANDROID(...)
#endif

#ifndef GEODE_PLATFORM_NAME
	#error "Unsupported PlatformID!"
#endif
