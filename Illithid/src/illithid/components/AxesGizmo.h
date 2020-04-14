#pragma once
#include <array>

#include "illithid/scene/Component.h"
#include "illithid/renderer/LineSegment.h"
#include "glm/glm.hpp"

namespace itd
{
	class AxesGizmo : public Component<AxesGizmo, 128>
	{
	public:
		virtual void OnStart( ) override;
		virtual void OnUpdate( ) override;
		virtual void OnPreRender( ) override;
		virtual void OnRender( ) override;
		virtual void OnPostRender( ) override;
	private:
		std::array<std::unique_ptr<LineSegment>, 3> axes_;
		std::array<glm::vec4, 3> colors_;
	};
}
