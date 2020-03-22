#pragma once
#include"illithid/renderer/Material.h"

namespace itd
{
	class Resources
	{
	public:
		static void Initialize( );

		static std::shared_ptr<Material> UnlitMaterial;
	};
}

