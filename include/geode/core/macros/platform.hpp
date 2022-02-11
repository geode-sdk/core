#pragma once

#if defined(GEODE_CALL)
	#undef GEODE_CALL
#endif

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
	#if TARGET_OS_IPHONE
		#define GEODE_MACOS(...)
		#define GEODE_IOS(...) __VA_ARGS__
		#define GEODE_IS_IOS
		#define GEODE_IS_MOBILE
		#define GEODE_PLATFORM_NAME "iOS"
		#define GEODE_PLATFORM_TARGET PlatformID::iOS
		#define GEODE_PLATFORM_EXTENSION ".dylib"
	#else
		#define GEODE_IOS(...)
		#define GEODE_MACOS(...) __VA_ARGS__
		#define GEODE_IS_MACOS
		#define GEODE_IS_DESKTOP
		#define GEODE_PLATFORM_NAME "MacOS"
		#define GEODE_PLATFORM_TARGET PlatformID::MacOS
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
