#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace itd
{
	enum class TransformationSpace : uint8_t
	{
		World, Local, Camera
	};

	class Transform
	{
	public:
		glm::vec3 Position;
		glm::quat Orientation;
		glm::vec3 ScaleFactor;

		Transform( );

		void Translate( const glm::vec3& displacement );
		void Rotate( const glm::vec3& euler, TransformationSpace space = TransformationSpace::World );
		void Scale( const glm::vec3& factor );

		void Update( );

		glm::vec3 Forward( ) const;
		glm::vec3 Right( ) const;
		glm::vec3 Up( ) const;

		inline glm::mat4 TRS( ) const
		{
			return TRS_;
		}

		inline glm::mat4 InverseTRS( ) const
		{
			return inverseTRS_;
		}

	private:
		glm::mat4 TRS_;
		glm::mat4 inverseTRS_;
	};

}