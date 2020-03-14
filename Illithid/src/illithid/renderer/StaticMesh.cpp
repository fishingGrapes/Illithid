#include "ilpch.h"
#include "StaticMesh.h"

#include "illithid\core\Log.h"
#include "glad/glad.h"

namespace itd
{
	StaticMesh::StaticMesh( std::vector<Vertex>&& vertices, std::vector<uint32_t>&& indices )
		: VAO_( 0 ), VBO_( 0 ), EBO_( 0 ), vertices_( std::move( vertices ) ), indices_( std::move( indices ) )
	{
		this->CreateVertexArrayObject( );
	}

	void StaticMesh::CreateVertexArrayObject( )
	{

		glCreateBuffers( 1, &VBO_ );
		glNamedBufferStorage( VBO_, sizeof( Vertex ) * vertices_.size( ), vertices_.data( ), 0 );

		glCreateBuffers( 1, &EBO_ );
		glNamedBufferStorage( EBO_, sizeof( uint32_t ) * indices_.size( ), indices_.data( ), 0 );

		glCreateVertexArrays( 1, &VAO_ );
		glVertexArrayVertexBuffer( VAO_, 0, VBO_, 0, sizeof( Vertex ) );
		glVertexArrayElementBuffer( VAO_, EBO_ );

		glEnableVertexArrayAttrib( VAO_, 0 );
		glEnableVertexArrayAttrib( VAO_, 1 );
		glEnableVertexArrayAttrib( VAO_, 2 );

		glVertexArrayAttribFormat( VAO_, 0, 3, GL_FLOAT, GL_FALSE, offsetof( Vertex, Position ) );
		glVertexArrayAttribFormat( VAO_, 1, 4, GL_FLOAT, GL_FALSE, offsetof( Vertex, Color ) );
		glVertexArrayAttribFormat( VAO_, 2, 2, GL_FLOAT, GL_FALSE, offsetof( Vertex, UV ) );

		glVertexArrayAttribBinding( VAO_, 0, 0 );
		glVertexArrayAttribBinding( VAO_, 1, 0 );
		glVertexArrayAttribBinding( VAO_, 2, 0 );

	}

	void StaticMesh::Bind( ) const
	{
		glBindVertexArray( VAO_ );
	}


}