#pragma once
#include "Illithid.h"





struct AxesLines
{
public:
	std::array<std::unique_ptr<LineSegment>, 3> lines;
	std::array<glm::vec4, 3> colors;

	explicit AxesLines( float_t scale )
	{
		lines[ 0 ] = std::make_unique<LineSegment>( glm::vec3( -scale, 0.0f, 0.0f ), glm::vec3( scale, 0.0f, 0.0f ) );
		lines[ 1 ] = std::make_unique<LineSegment>( glm::vec3( 0.0f, -scale, 0.0f ), glm::vec3( 0.0f, scale, 0.0f ) );
		lines[ 2 ] = std::make_unique<LineSegment>( glm::vec3( 0.0f, 0.0f, -scale ), glm::vec3( 0.0f, 0.0f, scale ) );

		colors[ 0 ] = glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f );
		colors[ 1 ] = glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f );
		colors[ 2 ] = glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f );
	}
};


struct MeshObject
{
public:
	Transform transform;
	std::shared_ptr<StaticMesh> mesh;
	AxesLines axes;

	MeshObject( )
		:axes( 1 )
	{

	}
};

struct LightObject
{
public:
	Transform transform;
	glm::vec4 color;
	std::shared_ptr<StaticMesh> mesh;
	AxesLines axes;

	LightObject( )
		:axes( 1 )
	{

	}
};

struct CameraObject
{
public:
	Transform transform;
	Camera camera;
};