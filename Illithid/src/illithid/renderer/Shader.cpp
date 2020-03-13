#include "ilpch.h"
#include"Shader.h"

#include "illithid/core/FileSystem.h"
#include "nlohmann/json.hpp"

namespace itd
{
	Shader::Shader( const std::string& path )
	{
		std::string shader = FileSystem::LoadAsText( path );
		nlohmann::json shaderObject = nlohmann::json::parse( shader );

		VertexSource = FileSystem::LoadAsText( shaderObject[ "vertex" ] );
		FragmentSource = FileSystem::LoadAsText( shaderObject[ "fragment" ] );
	}
}