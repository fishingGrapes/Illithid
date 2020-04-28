#include "ilpch.h"
#include"Shader.h"

#include "illithid/core/FileSystem.h"
#include "nlohmann/json.hpp"

namespace itd
{
	Shader::Shader( const std::string& path )
	{
		std::string shader = FileSystem::ReadAsText( path );
		nlohmann::json shaderObject = nlohmann::json::parse( shader );

		VertexSource = FileSystem::ReadAsText( shaderObject[ "vertex" ] );
		FragmentSource = FileSystem::ReadAsText( shaderObject[ "fragment" ] );
		ShaderQueue = static_cast<size_t>( shaderObject[ "queue" ] );
		HasLightingPass = static_cast<bool>( shaderObject[ "lighting_pass" ] );
	}
}