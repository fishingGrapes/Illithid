#pragma once
#include "Shader.h"
#include "glm/fwd.hpp"

namespace itd
{
	class Material
	{

	public:
		explicit Material( const Shader& shader );
		~Material( );

		Material( ) = delete;
		Material( const Material& other ) = delete;
		Material& operator=( const Material& other ) = delete;

		void Use( );
		void SetFloat( const char* uniform, float_t value );
		void SetVector2f( const char* uniform, glm::vec2& value );

	private:
		struct UniformData
		{
		public:
			int32_t Location;
			uint32_t Size;
		};

		using UniformMap = std::unordered_map<std::string, UniformData>;
		UniformMap uniformMap_;

		uint32_t program_;

		void CreateProgram( const Shader& shader );
		void CacheUniforms( );
	};
}
