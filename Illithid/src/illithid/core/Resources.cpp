#include "ilpch.h"
#include "Resources.h"

namespace itd
{
	std::shared_ptr<Material> Resources::UnlitMaterial;
	std::shared_ptr<StaticMesh> Resources::CubeMesh;

	void Resources::Initialize( )
	{
		Shader UnlitShader( "Assets/Shaders/color.shader" );
		UnlitMaterial = std::make_shared<Material>( UnlitShader );

		CubeMesh = StaticMesh::Load( "Assets/Models/cube.obj" );
	}
}
