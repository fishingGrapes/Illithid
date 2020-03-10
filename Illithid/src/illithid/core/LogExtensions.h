#pragma once

namespace glm
{
	inline std::ostream& operator<< ( std::ostream& os, const vec2& evnt )
	{
		return os << "(" << evnt.x << ",  " << evnt.y << ")";
	}
}
