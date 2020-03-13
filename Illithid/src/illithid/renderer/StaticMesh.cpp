#include "ilpch.h"
#include "StaticMesh.h"

#include "illithid\core\Log.h"
#include "glad/glad.h"

namespace itd
{
	StaticMesh::StaticMesh( std::vector<Vertex>&& vertices )
		: VAO_( 0 ), VBO_( 0 ), EBO_( 0 ), vertices_( std::move( vertices ) )
	{
		this->CreateVertexArrayObject( );
	}

	void StaticMesh::CreateVertexArrayObject( )
	{
		glGenVertexArrays( 1, &VAO_ );
		glBindVertexArray( VAO_ );

		glGenBuffers( 1, &VBO_ );
		glBindBuffer( GL_ARRAY_BUFFER, VBO_ );

		glBufferData( GL_ARRAY_BUFFER, sizeof( Vertex ) * vertices_.size( ), vertices_.data( ), GL_STATIC_DRAW );

		glEnableVertexAttribArray( 0 );
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*) offsetof( Vertex, Position ) );

		glEnableVertexAttribArray( 1 );
		glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*) offsetof( Vertex, Color ) );
	}

	void StaticMesh::Bind( ) const
	{
		glBindVertexArray( VAO_ );
	}


}