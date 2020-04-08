#include "ilpch.h"
#include "GameObject.h"

#include "illithid/components/AxesGizmo.h"

namespace itd
{
	GameObject::GameObject( )
	{
		componentFilter_.reset( );
		transform_ = this->AddComponent<Transform>( );

#if defined(IL_DEBUG) || defined(IL_RELEASE)
		auto& gizmo =  this->AddComponent<AxesGizmo>( );
#endif

		SceneGraph::Add( this );
	}

	GameObject::~GameObject( )
	{
		SceneGraph::Remove( this );

		components_.clear( );
		componentMap_.clear( );
		componentFilter_.reset( );
	}

	void GameObject::Update( )
	{
		for each (std::shared_ptr<ComponentBase> component in components_)
		{
			component->OnUpdate( );
		}
	}

	void GameObject::PreRender( )
	{
		for each (std::shared_ptr<ComponentBase> component in components_)
		{
			component->OnPreRender( );
		}
	}

	void GameObject::Render( )
	{
		for each (std::shared_ptr<ComponentBase> component in components_)
		{
			component->OnRender( );
		}
	}

	void GameObject::PostRender( )
	{
		for each (std::shared_ptr<ComponentBase> component in components_)
		{
			component->OnPostRender( );
		}
	}
}