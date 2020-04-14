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

	class Light : public Component<Light, 32>
	{
	public:

		Light( LightType type = LightType::Directional )
			: Type( type ), Color( glm::vec4( 1.0f ) )
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