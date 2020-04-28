#pragma once

namespace itd
{
	struct Shader
	{
	public:
		explicit Shader( const std::string& shader );
		Shader() = delete;

		std::string VertexSource;
		std::string FragmentSource;
		size_t ShaderQueue;
		bool HasLightingPass;
	};
}
