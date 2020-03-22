#include "ilpch.h"
#include "AxesGizmo.h"

#include "illithid/core/Resources.h"
#include "illithid/renderer/Graphics.h"
#include "illithid/scene/GameObject.h"
#include "illithid/components/Camera.h"

namespace itd
{
	void AxesGizmo::OnStart( )
	{
		axes_[ 0 ] = std::make_unique<LineSegment>( glm::vec3( -1.0f, 0.0f, 0.0f ), glm::vec3( 1.0f, 0.0f, 0.0f ) );
		axes_[ 1 ] = std::make_unique<LineSegment>( glm::vec3( 0.0f, -1.0f, 0.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
		axes_[ 2 ] = std::make_unique<LineSegment>( glm::vec3( 0.0f, 0.0f, -1.0f ), glm::vec3( 0.0f, 0.0f, 1.0f ) );

		colors_[ 0 ] = glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f );
		colors_[ 1 ] = glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f );
		colors_[ 2 ] = glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f );
	}

	void AxesGizmo::OnUpdate( )
	{
	}

	void AxesGizmo::OnPreRender( )
	{
		Resources::UnlitMaterial->SetMatrix4f( "u_ViewProjection", Camera::Primary( )->ViewProjection( ) );
	}

	void AxesGizmo::OnRender( )
	{
		Resources::UnlitMaterial->SetMatrix4f( "u_Model", gameObject_->GetTransform( )->TRS( ) );
		for (size_t i = 0; i < axes_.size( ); i++)
		{
			Resources::UnlitMaterial->SetVector4f( "u_Color", colors_[ i ] );
			Graphics::DrawLineSegment( *axes_[ i ], *Resources::UnlitMaterial );
		}
	}

	void AxesGizmo::OnPostRender( )
	{

	}
}