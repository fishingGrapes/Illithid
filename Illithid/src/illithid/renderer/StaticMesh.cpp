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
		glCreateVertexArrays( 1, &VAO_ );

		glCreateBuffers( 1, &VBO_ );
		glNamedBufferStorage( VBO_, sizeof( Vertex ) * vertices_.size( ), vertices_.data( ), 0 );

		glVertexArrayVertexBuffer( VAO_, 0, VBO_, 0, sizeof( Vertex ) );

		glEnableVertexArrayAttrib( VAO_, 0 );
		glEnableVertexArrayAttrib( VAO_, 1 );

		glVertexArrayAttribFormat( VAO_, 0, 3, GL_FLOAT, GL_FALSE, offsetof( Vertex, Position ) );
		glVertexArrayAttribFormat( VAO_, 1, 4, GL_FLOAT, GL_FALSE, offsetof( Vertex, Color ) );

		glVertexArrayAttribBinding( VAO_, 0, 0 );
		glVertexArrayAttribBinding( VAO_, 1, 0 );

		/*glGenVertexArrays( 1, &VAO_ );
		glBindVertexArray( VAO_ );

		glGenBuffers( 1, &VBO_ );
		glBindBuffer( GL_ARRAY_BUFFER, VBO_ );

		glBufferData( GL_ARRAY_BUFFER, sizeof( Vertex ) * vertices_.size( ), vertices_.data( ), GL_STATIC_DRAW );

		glEnableVertexAttribArray( 0 );
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*) offsetof( Vertex, Position ) );

		glEnableVertexAttribArray( 1 );
		glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*) offsetof( Vertex, Color ) );*/
	}

	void StaticMesh::Bind( ) const
	{
		glBindVertexArray( VAO_ );
	}


}