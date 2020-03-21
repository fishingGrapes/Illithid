#pragma once
#include "glm/fwd.hpp"

namespace itd
{
	class LineSegment
	{
	public:
		explicit LineSegment( const glm::vec3& begin, const glm::vec3& end );
		~LineSegment( );

		void Bind() const;

	private:
		uint32_t VBO_, VAO_;
		void CreateVertexArrayObject( );

		std::vector<glm::vec3> vertices_;
	};
}