#pragma once
#include "illithid/scene/Component.h"
#include "glm/glm.hpp"

namespace itd
{
	enum class LightType
	{
		Directional,
		Point,
		Spot
	};

	class Light : public Component<Light>
	{
	public:

		explicit Light( LightType type ) 
			: Type( type )
		{

		}

		glm::vec4 Color;
		LightType Type;

		virtual void OnStart( ) override;
		virtual void OnUpdate( ) override;
		virtual void OnPreRender( ) override;
		virtual void OnRender( ) override;
		virtual void OnPostRender( ) override;

	};
}