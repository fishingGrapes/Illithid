#pragma once
#include <chrono>

namespace itd
{
	struct Time
	{
		friend class Application;

	public:
		inline static float_t Elapsed( )
		{
			return elapsed_;

		}
		inline static float_t Delta( )
		{
			return delta_;
		}

	private:
		static float_t elapsed_;
		static float_t delta_;

	};
}