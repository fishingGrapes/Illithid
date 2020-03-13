#pragma once
#include "Shader.h"

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
	private:
		uint32_t program_;

		void CreateProgram( const Shader& shader );
	};
}
