#pragma once
#include "illithid/scene/Component.h"
#include "glm/glm.hpp"

namespace itd
{
	class Light : public Component<Light>
	{
	public:
		glm::vec4 Color;

		virtual void OnStart( ) override;
		virtual void OnUpdate( ) override;
		virtual void OnPreRender( ) override;
		virtual void OnRender( ) override;
		virtual void OnPostRender( ) override;
	};
}