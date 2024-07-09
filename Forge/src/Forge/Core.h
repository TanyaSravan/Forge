#pragma once

#ifdef FG_PLATFORM_WINDOWS
	#ifdef FG_BUILD_DLL
		#define FORGE_API __declspec(dllexport)
	#else 
		#define FORGE_API __declspec(dllimport)
	#endif 
#else
	#error FORGE RUNS ONLY ON WINDOWS
#endif

#define BIT(x) (1<<x)

#ifdef FG_ENABLE_ASSERTS
	#define FG_ASSERT(x,...) {if(!(x)) {FG_ERROR("Assertion failed: {0}", __VA_ARGS__),__debugbreak();}}
	#define FG_CORE_ASSERT(x,...) {if(!(x)) {FG_ERROR("Assertion failed: {0}", __VA_ARGS__),__debugbreak();}}
#else
	#define FG_ASSERT(x,...)
	#define FG_CORE_ASSERT(x,...)
#endif


