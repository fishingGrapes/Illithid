#pragma once
#include "glm/fwd.hpp"
#include "Shader.h"

namespace itd
{
	class Texture2D;
	class Material
	{

	public:
		explicit Material( const Shader& shader );
		~Material( );

		Material( ) = delete;
		Material( const Material& other ) = delete;
		Material& operator=( const Material& other ) = delete;

		void SetTexture( const std::string& uniform, std::shared_ptr<Texture2D> texture );

		void Use( );
		void SetInt( const char* uniform, int32_t value );
		void SetFloat( const char* uniform, float_t value );
		void SetVector2f( const char* uniform, const glm::vec2& value );
		void SetVector3f( const char* uniform, const glm::vec3& value );
		void SetVector4f( const char* uniform, const glm::vec4& value );
		void SetMatrix3f( const char* uniform, const glm::mat3& value );
		void SetMatrix4f( const char* uniform, const glm::mat4& value );

		inline uint32_t ProgramID( ) const
		{
			return program_;
		}

		const bool HasLightingPass;
		const size_t ShaderQueue;

	private:
		struct UniformData
		{
		public:
			int32_t Location;
			uint32_t Size;
		};

		struct TextureData
		{
		public:
			std::shared_ptr<Texture2D> Texture;
			uint32_t Unit;
		};

		using UniformMap = std::unordered_map<std::string, UniformData>;
		UniformMap uniformMap_;

		using TextureMap = std::unordered_map<std::string, TextureData>;
		TextureMap textureMap_;

		uint32_t program_;

		void CreateProgram( const Shader& shader );
		void CacheUniforms( );
	};
}
