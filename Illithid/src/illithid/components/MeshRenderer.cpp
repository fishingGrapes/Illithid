#include "ilpch.h"
#include "MeshRenderer.h"

#include "illithid/renderer/Graphics.h"
#include "illithid/game/GameObject.h"
#include "illithid/components/Camera.h"
#include "illithid/components/Light.h"
#include "illithid/core/Log.h"

namespace itd
{

	void MeshRenderer::OnPreRender( )
	{
		Material->SetMatrix4f( "u_ViewProjection", Camera::Primary( )->ViewProjection( ) );
		Material->SetMatrix4f( "u_Model", gameObject->GetTransform( )->TRS( ) );
	}

	void MeshRenderer::OnRender( )
	{
		//Graphics::DrawMesh( *Mesh, *Material );
	}

	void MeshRenderer::OnStart( )
	{
	}

	void MeshRenderer::OnUpdate( )
	{
	}

	void MeshRenderer::OnPostRender( )
	{
	}

}