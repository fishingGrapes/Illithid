#include "ilpch.h"
#include "LogExtensions.h"

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace glm
{

	std::ostream& operator<< ( std::ostream& os, const vec2& obj )
	{
		return os << "(" << obj.x << ",  " << obj.y << ")";
	}

	std::ostream& operator<< ( std::ostream& os, const vec3& obj )
	{
		return os << "(" << obj.x << ",  " << obj.y << ",  " << obj.z << ")";
	}

	std::ostream& operator<< ( std::ostream& os, const vec4& obj )
	{
		return os << "(" << obj.x << ",  " << obj.y << ",  " << obj.z << ",  " << obj.w << ")";
	}

	std::ostream& operator<< ( std::ostream& os, const quat& obj )
	{
		return os << "(" << obj.w << ",  " << obj.x << ",  " << obj.y << ",  " << obj.z << ")";
	}
}