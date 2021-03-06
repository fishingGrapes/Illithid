#pragma once
#include <vector>

#include "Vertex.h"

namespace itd
{
	class StaticMesh
	{
	public:
		explicit StaticMesh( std::vector<Vertex>&& vertices );
		~StaticMesh( );

		StaticMesh( const StaticMesh& other );
		StaticMesh& operator=( const StaticMesh& other );

		void Bind( ) const;

		inline size_t VertexCount( ) const
		{
			return  vertices_.size( );
		}

		StaticMesh( ) = default;

		static std::shared_ptr<StaticMesh> Load( const std::string& path );

	private:
		uint32_t VAO_, VBO_;
		std::vector<Vertex> vertices_;

		void CreateVertexArrayObject( );
	};
}