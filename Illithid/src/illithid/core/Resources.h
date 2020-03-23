#pragma once
#include "illithid/renderer/Material.h"
#include "illithid/renderer/StaticMesh.h"

namespace itd
{
	class Resources
	{
	public:
		static void Initialize( );

		static std::shared_ptr<Material> UnlitMaterial;
		static std::shared_ptr<StaticMesh> CubeMesh;

	};
}

