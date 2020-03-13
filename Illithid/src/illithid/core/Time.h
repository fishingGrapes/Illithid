#pragma once
#include <chrono>

namespace itd
{
	struct Time
	{
		friend class Application;

	public:
		inline static double_t Elapsed( )
		{
			return elapsed_;
		}

	private:
		static double_t elapsed_;
	};
}