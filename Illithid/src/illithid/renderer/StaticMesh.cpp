#include "ilpch.h"
#include "StaticMesh.h"

#include "illithid/core/Log.h"
#include "glad/glad.h"
#include "illithid/core/FileSystem.h"

#include "tiny_obj_loader.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace itd
{
	StaticMesh::StaticMesh( std::vector<Vertex>&& vertices )
		: VAO_( 0 ), VBO_( 0 ), vertices_( std::move( vertices ) )
	{


		this->CreateVertexArrayObject( );
	}

	StaticMesh::~StaticMesh( )
	{
		glDeleteBuffers( 1, &VBO_ );
		glDeleteVertexArrays( 1, &VAO_ );
	}


	void StaticMesh::CreateVertexArrayObject( )
	{
		glCreateBuffers( 1, &VBO_ );
		glNamedBufferStorage( VBO_, sizeof( Vertex ) * vertices_.size( ), vertices_.data( ), 0 );

		glCreateVertexArrays( 1, &VAO_ );
		glVertexArrayVertexBuffer( VAO_, 0, VBO_, 0, sizeof( Vertex ) );

		glEnableVertexArrayAttrib( VAO_, 0 );
		glEnableVertexArrayAttrib( VAO_, 1 );
		glEnableVertexArrayAttrib( VAO_, 2 );
		glEnableVertexArrayAttrib( VAO_, 3 );

		glVertexArrayAttribFormat( VAO_, 0, 3, GL_FLOAT, GL_FALSE, offsetof( Vertex, Position ) );
		glVertexArrayAttribFormat( VAO_, 1, 4, GL_FLOAT, GL_FALSE, offsetof( Vertex, Color ) );
		glVertexArrayAttribFormat( VAO_, 2, 2, GL_FLOAT, GL_FALSE, offsetof( Vertex, UV ) );
		glVertexArrayAttribFormat( VAO_, 3, 3, GL_FLOAT, GL_FALSE, offsetof( Vertex, Normal ) );

		glVertexArrayAttribBinding( VAO_, 0, 0 );
		glVertexArrayAttribBinding( VAO_, 1, 0 );
		glVertexArrayAttribBinding( VAO_, 2, 0 );
		glVertexArrayAttribBinding( VAO_, 3, 0 );
	}

	void StaticMesh::Bind( ) const
	{
		glBindVertexArray( VAO_ );
	}


	std::shared_ptr<StaticMesh> StaticMesh::Load( const std::string& path )
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile( FileSystem::GetAbsolutePath( path ), aiProcess_Triangulate | aiProcess_FlipUVs );
		delete scene;

		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string warning, error;

		bool success = tinyobj::LoadObj( &attrib, &shapes, &materials, &warning, &error, path.c_str( ) );

		if (!warning.empty( ))
		{
			IL_CORE_WARN( warning );
		}

		if (!error.empty( ))
		{
			IL_CORE_ERROR( error );
		}

		if (!success)
		{
			IL_CORE_ERROR( "Tinyobjloader failed to load mesh {0}", path );
			return std::shared_ptr<StaticMesh>( );
		}


		std::vector<Vertex> vertices;

		for (size_t s = 0; s < shapes.size( ); s++)
		{
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[ s ].mesh.num_face_vertices.size( ); f++)
			{
				int fv = shapes[ s ].mesh.num_face_vertices[ f ];

				for (size_t v = 0; v < fv; v++)
				{
					tinyobj::index_t idx = shapes[ s ].mesh.indices[ index_offset + v ];

					tinyobj::real_t x = attrib.vertices[ 3 * static_cast<int64_t>( idx.vertex_index ) + 0 ];
					tinyobj::real_t y = attrib.vertices[ 3 * static_cast<int64_t>( idx.vertex_index ) + 1 ];
					tinyobj::real_t z = attrib.vertices[ 3 * static_cast<int64_t>( idx.vertex_index ) + 2 ];

					tinyobj::real_t r;
					tinyobj::real_t g;
					tinyobj::real_t b;
					if (!attrib.colors.empty( ))
					{
						r = attrib.colors[ 3 * static_cast<int64_t>( idx.vertex_index ) + 0 ];
						g = attrib.colors[ 3 * static_cast<int64_t>( idx.vertex_index ) + 1 ];
						b = attrib.colors[ 3 * static_cast<int64_t>( idx.vertex_index ) + 2 ];
					}
					else
					{
						r = g = b = 1.0f;
					}

					tinyobj::real_t s;
					tinyobj::real_t t;
					if (!attrib.texcoords.empty( ))
					{
						s = attrib.texcoords[ 2 * static_cast<int64_t>( idx.texcoord_index ) + 0 ];
						t = attrib.texcoords[ 2 * static_cast<int64_t>( idx.texcoord_index ) + 1 ];
					}
					else
					{
						s = t = 0.0f;
					}

					tinyobj::real_t nx;
					tinyobj::real_t ny;
					tinyobj::real_t nz;
					if (!attrib.normals.empty( ))
					{
						nx = attrib.normals[ 3 * static_cast<int64_t>( idx.normal_index ) + 0 ];
						ny = attrib.normals[ 3 * static_cast<int64_t>( idx.normal_index ) + 1 ];
						nz = attrib.normals[ 3 * static_cast<int64_t>( idx.normal_index ) + 2 ];
					}
					else
					{
						nx = ny = nz = 0.0f;
					}

					vertices.emplace_back( Vertex{ glm::vec3( x, y, z ), glm::vec4( r, g, b, 1.0f ), glm::vec2( s, t ) , glm::vec3( nx, ny, nz ) } );
				}

				index_offset += fv;
			}
		}


		std::shared_ptr<StaticMesh> mesh = std::make_shared<StaticMesh>( std::move( vertices ) );
		return mesh;
	}


}