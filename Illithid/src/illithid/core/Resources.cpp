#include "ilpch.h"
#include "Resources.h"

namespace itd
{
	std::shared_ptr<Material> Resources::UnlitMaterial;

	void Resources::Initialize( )
	{
		Shader colorShader( "Assets/Shaders/color.shader" );
		UnlitMaterial = std::make_unique<Material>( colorShader );
	}
}
