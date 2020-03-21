#include "ilpch.h"
#include "LineSegment.h"

#include "glad/glad.h"
#include "glm/glm.hpp"

namespace itd
{
	LineSegment::LineSegment( const glm::vec3& begin, const glm::vec3& end )
		:VBO_( 0 ), VAO_( 0 )
	{
		vertices_.push_back( begin );
		vertices_.push_back( end );

		this->CreateVertexArrayObject( );
	}

	LineSegment::~LineSegment( )
	{
		glDeleteBuffers( 1, &VBO_ );
		glDeleteVertexArrays( 1, &VAO_ );
	}

	void LineSegment::Bind( ) const
	{
		glBindVertexArray( VAO_ );
	}

	void LineSegment::CreateVertexArrayObject( )
	{
		glCreateBuffers( 1, &VBO_ );
		glNamedBufferStorage( VBO_, sizeof( glm::vec3 ) * vertices_.size( ), vertices_.data( ), 0 );

		glCreateVertexArrays( 1, &VAO_ );
		glVertexArrayVertexBuffer( VAO_, 0, VBO_, 0, sizeof( glm::vec3 ) );

		glEnableVertexArrayAttrib( VAO_, 0 );
		glVertexArrayAttribFormat( VAO_, 0, 3, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayAttribBinding( VAO_, 0, 0 );
	}
}