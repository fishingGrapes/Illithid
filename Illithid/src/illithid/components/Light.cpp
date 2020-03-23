#include "ilpch.h"
#include "Light.h"

#include "illithid/scene/GameObject.h"
#include "illithid/components/MeshRenderer.h"
#include "illithid/components/Camera.h"
#include "illithid/core/Resources.h"
#include "illithid/renderer/Graphics.h"

namespace itd
{
	void Light::OnStart( )
	{
#if defined(IL_DEBUG) || defined(IL_RELEASE)
		if (Type == LightType::Point)
		{
			auto renderer = gameObject_->AddComponent<MeshRenderer>( );
			renderer->Material = Resources::UnlitMaterial;
			renderer->Mesh = Resources::CubeMesh;
		}
#endif
	}

	void Light::OnUpdate( )
	{
	}

	void Light::OnPreRender( )
	{
#if defined(IL_DEBUG) || defined(IL_RELEASE)
		if (Type == LightType::Point)
		{
			gameObject_->GetComponent<MeshRenderer>( )->Material->SetMatrix4f( "u_ViewProjection", Camera::Primary( )->ViewProjection( ) );
		}
#endif

	}

	void Light::OnRender( )
	{
#if defined(IL_DEBUG) || defined(IL_RELEASE)
		if (Type == LightType::Point)
		{
			auto renderer = gameObject_->GetComponent<MeshRenderer>( );
			renderer->Material->SetMatrix4f( "u_Model", gameObject_->GetTransform( )->TRS( ) );
			renderer->Material->SetVector4f( "u_Color", Color );

			Graphics::DrawMesh( *renderer->Mesh, *renderer->Material );
		}
#endif
	}

	void Light::OnPostRender( )
	{
	}
}