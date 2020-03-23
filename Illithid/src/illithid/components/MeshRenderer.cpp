#include "ilpch.h"
#include "MeshRenderer.h"

#include "illithid/renderer/Graphics.h"
#include "illithid/scene/GameObject.h"
#include "illithid/components/Camera.h"
#include "illithid/components/Light.h"

namespace itd
{
	void MeshRenderer::OnPreRender( )
	{
		Material->SetMatrix4f( "u_ViewProjection", Camera::Primary( )->ViewProjection( ) );
	}

	void MeshRenderer::OnRender( )
	{
		Material->SetMatrix4f( "u_Model", gameObject_->GetTransform( )->TRS( ) );

		Graphics::DrawMesh( *Mesh, *Material );
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