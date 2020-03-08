#pragma once

#include "spdlog/logger.h"

namespace itd
{
	class Log
	{
	public:
		Log() = delete;
		static void Initialize( );

		inline static std::shared_ptr<spdlog::logger> GetCoreLogger( )
		{
			return coreLogger_;
		};

		inline static std::shared_ptr<spdlog::logger> GetClientLogger( )
		{
			return clientLogger_;
		};

	private:
		static std::shared_ptr<spdlog::logger> coreLogger_;
		static std::shared_ptr<spdlog::logger> clientLogger_;

	};
}

#if defined(IL_DEBUG) || defined(IL_RELEASE)

#define IL_CORE_TRACE(...)	    ::itd::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define IL_CORE_INFO(...)       ::itd::Log::GetCoreLogger()->info(__VA_ARGS__)
#define IL_CORE_WARN(...)       ::itd::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define IL_CORE_ERROR(...)      ::itd::Log::GetCoreLogger()->error(__VA_ARGS__)
#define IL_CORE_CRITICAL(...)   ::itd::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define IL_TRACE(...)	   ::itd::Log::GetClientLogger()->trace(__VA_ARGS__)
#define IL_INFO(...)       ::itd::Log::GetClientLogger()->info(__VA_ARGS__)
#define IL_WARN(...)       ::itd::Log::GetClientLogger()->warn(__VA_ARGS__)
#define IL_ERROR(...)      ::itd::Log::GetClientLogger()->error(__VA_ARGS__)
#define IL_CRITICAL(...)   ::itd::Log::GetClientLogger()->critical(__VA_ARGS__)

#else

#define IL_CORE_TRACE(...)	    
#define IL_CORE_INFO(...)       
#define IL_CORE_WARN(...)       
#define IL_CORE_ERROR(...)      
#define IL_CORE_CRITICAL(...)   


#define IL_TRACE(...)	   
#define IL_INFO(...)       
#define IL_WARN(...)       
#define IL_ERROR(...)      
#define IL_CRITICAL(...)   


#endif