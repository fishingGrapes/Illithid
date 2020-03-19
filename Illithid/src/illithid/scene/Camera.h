#pragma once
#include "glm/glm.hpp"

namespace itd
{

	struct OrthographicProjection
	{
	public:
		float_t Left;
		float_t Right;
		float_t Bottom;
		float_t Top;

		float_t Near;
		float_t Far;
	};

	struct PerspectiveProjection
	{
	public:
		float_t FOV;
		float_t AspectRatio;

		float_t Near;
		float_t Far;
	};

	class Camera
	{
	public:

		Camera( );

		void SetPerspectiveProjection( const PerspectiveProjection& projection );
		void SetOrthographicProjection( const OrthographicProjection& projection );

		glm::mat4 CalculateViewProjection( const glm::mat4& viewMatrix );

	private:
		glm::mat4 projectionMatrix_;
	};
}
