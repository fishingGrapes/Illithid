#pragma once
#include "illithid/game/Component.h"
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
			: Type( type ), Color( glm::vec3( 1.0f ) )
		{

		}

		glm::vec3 Color;
		const LightType Type;

		virtual void OnStart( ) override;
		virtual void OnUpdate( ) override;
		virtual void OnPreRender( ) override;
		virtual void OnRender( ) override;
		virtual void OnPostRender( ) override;

	};
}