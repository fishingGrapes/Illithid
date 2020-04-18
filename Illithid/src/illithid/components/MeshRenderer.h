#pragma once
#include <memory>
#include "illithid/game/Component.h"
#include "illithid/renderer/Material.h"
#include "illithid/renderer/StaticMesh.h"


namespace itd
{
	class MeshRenderer : public Component<MeshRenderer, 128>
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