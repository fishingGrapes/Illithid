#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace itd
{
	class Transform
	{
	public:
		glm::vec3 Position;
		glm::quat Orientation;
		glm::vec3 ScaleFactor;

		Transform( );

		void Translate( const glm::vec3& displacement );
		void Rotate( const glm::vec3& euler );
		void Scale( const glm::vec3& factor );

		void Update();

		inline glm::mat4 TRS( ) const
		{
			return TRS_;
		}

	private:
		glm::mat4 TRS_;
	
	};

}