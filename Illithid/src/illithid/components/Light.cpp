#include "ilpch.h"
#include "Light.h"

#include "illithid/game/GameObject.h"
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
			auto& renderer = gameObject->AddComponent<MeshRenderer>( );
			renderer->Mesh = Resources::CubeMesh;
			renderer->Material = std::make_shared<Material>( Shader( "Assets/Shaders/color.shader" ) );
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
			auto& renderer = gameObject->GetComponent<MeshRenderer>( );
			renderer->Material->SetVector4f( "u_Color", glm::vec4( Color, 1.0f ) );
		}
#endif

	}

	void Light::OnRender( )
	{

	}

	void Light::OnPostRender( )
	{
	}
}