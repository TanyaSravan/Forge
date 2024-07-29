#pragma once
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Forge {
	class FORGE_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return m_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return m_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> m_CoreLogger;
		static std::shared_ptr<spdlog::logger> m_ClientLogger;
	};

}
//(...) means a variable number of arguments and __VA_ARGS__ forwards the variable args we recieve
//Core Log Macros
#define FG_CORE_FATAL(...)   ::Forge::Log::GetCoreLogger()->fatal(__VA_ARGS__)
#define FG_CORE_ERROR(...)   ::Forge::Log::GetCoreLogger()->error(__VA_ARGS__)
#define FG_CORE_WARN(...)    ::Forge::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define FG_CORE_INFO(...)    ::Forge::Log::GetCoreLogger()->info(__VA_ARGS__)
#define FG_CORE_TRACE(...)    ::Forge::Log::GetCoreLogger()->trace(__VA_ARGS__)

//Client Log Macros
#define FG_FATAL(...) ::Forge::Log::GetClientLogger()->fatal(__VA_ARGS__)
#define FG_ERROR(...) ::Forge::Log::GetClientLogger()->error(__VA_ARGS__)
#define FG_WARN(...)  ::Forge::Log::GetClientLogger()->warn(__VA_ARGS__)
#define FG_INFO(...)  ::Forge::Log::GetClientLogger()->info(__VA_ARGS__)
#define FG_TRACE(...)  ::Forge::Log::GetClientLogger()->trace(__VA_ARGS__)
