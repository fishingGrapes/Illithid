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
		auto& gizmo = this->AddComponent<AxesGizmo>( );
#endif
	}

	GameObject::~GameObject( )
	{
		for (DeleterMap::iterator it = deleterMap_.begin( ); it != deleterMap_.end( ); ++it)
		{
			it->second( );
		}

		deleterMap_.clear( );
		componentMap_.clear( );
		componentFilter_.reset( );
	}
}