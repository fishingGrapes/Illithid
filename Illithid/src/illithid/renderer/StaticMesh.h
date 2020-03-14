#pragma once
#include <vector>

#include "Vertex.h"

namespace itd
{
	class StaticMesh
	{
	public:
		explicit StaticMesh( std::vector<Vertex>&& vertices, std::vector<uint32_t>&& indices );

		void Bind( ) const;

		inline size_t VertexCount( ) const
		{
			return  vertices_.size( );
		}

		inline size_t ElementCount( ) const
		{
			return  indices_.size( );
		}

		StaticMesh( ) = delete;
		StaticMesh( const StaticMesh& other ) = delete;
		StaticMesh& operator=( const StaticMesh& other ) = delete;

	private:
		uint32_t VAO_, VBO_, EBO_;

		const std::vector<Vertex> vertices_;
		const std::vector<uint32_t> indices_;

		void CreateVertexArrayObject( );
	};
}