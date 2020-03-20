#pragma once
#include "glm/glm.hpp"

namespace itd
{
	struct Vertex
	{
	public:
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 UV;
		glm::vec3 Normal;
	};
}
