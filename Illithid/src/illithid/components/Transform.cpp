#include "ilpch.h"
#include "Transform.h"

#include "illithid/core/Log.h"
#include "illithid/core/LogExtensions.h"

#include "glm/gtc/matrix_transform.hpp"

namespace itd
{
	Transform::Transform( )
		: Position( glm::vec3( 0.0f ) ), Orientation( glm::quat( 1.0f, 0.0f, 0.0f, 0.0f ) ), ScaleFactor( glm::vec3( 1.0f ) ),
		TRS_( glm::mat4( 1.0f ) ), inverseTRS_( glm::mat4( 1.0f ) )
	{
	}


	void Transform::Translate( const glm::vec3& displacement )
	{
		Position += displacement;
	}

	void Transform::Rotate( const glm::vec3& euler, TransformationSpace space )
	{
		glm::quat& delta_x = glm::angleAxis( euler.x, glm::vec3( 1.0f, 0.0f, 0.0f ) );	//pitch
		glm::quat& delta_y = glm::angleAxis( euler.y, glm::vec3( 0.0f, 1.0f, 0.0f ) );	//yaw
		glm::quat& delta_z = glm::angleAxis( euler.z, glm::vec3( 0.0f, 0.0f, 1.0f ) );	//roll

		switch (space)
		{
			case TransformationSpace::World:
				Orientation = delta_x * Orientation;
				Orientation = delta_y * Orientation;
				Orientation = delta_z * Orientation;
				break;

			case TransformationSpace::Local:
				Orientation = Orientation * delta_x;
				Orientation = Orientation * delta_y;
				Orientation = Orientation * delta_z;
				break;

			case TransformationSpace::Camera:
				Orientation = delta_y * Orientation;
				Orientation = Orientation * delta_x;
				Orientation = Orientation * delta_z;
				break;

		}

		Orientation = glm::normalize( Orientation );
	}

	void Transform::Scale( const glm::vec3& factor )
	{
		ScaleFactor *= factor;
	}

	glm::vec3 Transform::Forward( ) const
	{
		return Orientation * glm::vec3( 0.0f, 0.0f, -1.0f );
	}

	glm::vec3 Transform::Right( ) const
	{
		return Orientation * glm::vec3( 1.0f, 0.0f, 0.0f );
	}

	glm::vec3 Transform::Up( ) const
	{
		return Orientation * glm::vec3( 0.0f, 1.0f, 0.0f );
	}

	void Transform::OnStart( )
	{
	}

	void Transform::OnUpdate( )
	{
		glm::mat4 translateMatrix = glm::mat4( 1.0f );
		translateMatrix = glm::translate( translateMatrix, Position );

		glm::mat4 rotateMatrix = glm::mat4_cast( Orientation );

		glm::mat4 scaleMatrix = glm::mat4( 1.0f );
		scaleMatrix = glm::scale( scaleMatrix, ScaleFactor );

		TRS_ = translateMatrix * rotateMatrix * scaleMatrix;
		inverseTRS_ = glm::inverse( TRS_ );
	}

	void Transform::OnPreRender( )
	{
	}

	void Transform::OnRender( )
	{
	}

	void Transform::OnPostRender( )
	{
	}

}