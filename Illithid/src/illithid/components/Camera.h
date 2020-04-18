#pragma once
#include "glm/glm.hpp"
#include "illithid/game/Component.h"

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

	class Camera : public Component<Camera, 4>
	{
	public:

		Camera( );

		void SetPerspectiveProjection( const PerspectiveProjection& projection );
		void SetOrthographicProjection( const OrthographicProjection& projection );

		glm::mat4 ViewProjection( );

		virtual void OnStart( ) override;
		virtual void OnUpdate( ) override;
		virtual void OnPreRender( ) override;
		virtual void OnRender( ) override;
		virtual void OnPostRender( ) override;


		static void SetAsPrimary( dptr<Camera> camera )
		{
			primaryCamera_ = camera;
		}

		inline static dptr<Camera> Primary( )
		{
			return primaryCamera_;
		}

	private:
		glm::mat4 projectionMatrix_;
		static dptr<Camera> primaryCamera_;
	};
}
