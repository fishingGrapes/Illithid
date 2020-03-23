#include "ilpch.h"
#include "Resources.h"

namespace itd
{
	std::shared_ptr<Material> Resources::UnlitMaterial;
	std::shared_ptr<StaticMesh> Resources::CubeMesh;

	void Resources::Initialize( )
	{
		Shader colorShader( "Assets/Shaders/color.shader" );
		UnlitMaterial = std::make_shared<Material>( colorShader );

		CubeMesh = StaticMesh::Load( "Assets/Models/box.obj" );
	}
}
