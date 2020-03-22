#pragma once
#include <memory>

#include "illithid/scene/Component.h"
#include "illithid/renderer/Material.h"
#include "illithid/renderer/StaticMesh.h"


namespace itd
{
	class MeshRenderer : public Component<MeshRenderer>
	{
	public:
		std::shared_ptr<Material> Material;
		std::shared_ptr<StaticMesh> Mesh;

		virtual void OnStart( ) override;
		virtual void OnUpdate( ) override;
		virtual void OnPreRender( ) override;
		virtual void OnRender( ) override;
		virtual void OnPostRender( ) override;

	};
}