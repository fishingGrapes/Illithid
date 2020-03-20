#pragma once
#include "glm/fwd.hpp"

namespace glm
{
	std::ostream& operator<< ( std::ostream& os, const vec2& obj );
	std::ostream& operator<< ( std::ostream& os, const vec3& obj );
	std::ostream& operator<< ( std::ostream& os, const vec4& obj );

	std::ostream& operator<< ( std::ostream& os, const quat& obj );
}
