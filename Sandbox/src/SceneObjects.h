#pragma once
#include "Illithid.h"


using Transform = itd::Transform;
using Camera = itd::Camera;
using StaticMesh = itd::StaticMesh;

struct MeshObject
{
public:
	Transform transform;
	std::shared_ptr<StaticMesh> mesh;
};

struct LightObject
{
public:
	Transform transform;
	glm::vec4 color;
	std::shared_ptr<StaticMesh> mesh;
};

struct CameraObject
{
public:
	Transform transform;
	Camera camera;
};