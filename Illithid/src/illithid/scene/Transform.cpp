#include "ilpch.h"
#include "Transform.h"

#include "illithid/core/Log.h"
#include "illithid/core/LogExtensions.h"

#include "glm/gtc/matrix_transform.hpp"

namespace itd
{
	Transform::Transform( )
		: Position( glm::vec3( 0.0f ) ), Orientation( glm::quat( 1.0f, 0.0f, 0.0f, 0.0f ) ), ScaleFactor( glm::vec3( 1.0f ) ),
		TRS_( glm::mat4( 1.0f ) )
	{
		IL_CORE_TRACE( "Position : {0} Rotation : {1} Scale: {2}", Position, Orientation, ScaleFactor );
	}

	void Transform::Translate( const glm::vec3& displacement )
	{
		Position += displacement;
	}

	void Transform::Rotate( const glm::vec3& euler )
	{
		glm::quat& delta_x = glm::angleAxis( euler.x, glm::vec3( 1.0f, 0.0f, 0.0f ) );	//pitch
		glm::quat& delta_y = glm::angleAxis( euler.y, glm::vec3( 0.0f, 1.0f, 0.0f ) );	//yaw
		glm::quat& delta_z = glm::angleAxis( euler.z, glm::vec3( 0.0f, 0.0f, 1.0f ) );	//roll

		Orientation = delta_x * Orientation;
		Orientation = delta_y * Orientation;
		Orientation = delta_z * Orientation;

		Orientation = glm::normalize( Orientation );
	}

	void Transform::Scale( const glm::vec3& factor )
	{
		ScaleFactor *= factor;
	}

	void Transform::Update( )
	{
		glm::mat4 translateMatrix = glm::mat4( 1.0f );
		translateMatrix = glm::translate( translateMatrix, Position );

		glm::mat4 rotateMatrix = glm::mat4_cast( Orientation );

		glm::mat4 scaleMatrix = glm::mat4( 1.0f );
		scaleMatrix = glm::scale( scaleMatrix, ScaleFactor );

		TRS_ = translateMatrix * rotateMatrix * scaleMatrix;
	}


}