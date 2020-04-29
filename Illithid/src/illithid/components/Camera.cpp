#include "ilpch.h"
#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"
#include "illithid/game/GameObject.h"

namespace itd
{
	dptr<Camera> Camera::primaryCamera_;

	Camera::Camera( )
		:projectionMatrix_( glm::mat4( 1.0f ) )
	{
	}

	void Camera::SetPerspectiveProjection( const PerspectiveProjection& projection )
	{
		projectionMatrix_ = glm::perspective<float_t>( projection.FOV, projection.AspectRatio, projection.Near, projection.Far );
	}

	void Camera::SetOrthographicProjection( const OrthographicProjection& projection )
	{
		projectionMatrix_ = glm::ortho<float_t>( projection.Left, projection.Right, projection.Top, projection.Bottom, projection.Near, projection.Far );
	}

	glm::mat4 Camera::ViewProjection( ) const
	{
		return ( projectionMatrix_ * gameObject->GetTransform( )->InverseTRS( ) );
	}

	glm::mat4 Camera::ViewMatrix( ) const
	{
		return gameObject->GetTransform( )->InverseTRS( );
	}


	void Camera::OnStart( )
	{
	}
	void Camera::OnUpdate( )
	{
	}
	void Camera::OnPreRender( )
	{
	}
	void Camera::OnRender( )
	{
	}
	void Camera::OnPostRender( )
	{
	}

}