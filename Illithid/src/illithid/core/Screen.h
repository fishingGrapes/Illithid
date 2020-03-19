#pragma once
#include <stdint.h>

namespace itd
{
	struct Screen
	{
	public:
		static inline int32_t Width( )
		{
			return width_;
		}
		static inline int32_t Height( )
		{
			return height_;
		}

	private:

		friend class Application;

		static int32_t width_;
		static int32_t height_;
	};
}
