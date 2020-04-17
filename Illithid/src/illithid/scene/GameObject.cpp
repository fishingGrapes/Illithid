#include "ilpch.h"
#include "GameObject.h"

#include "illithid/components/AxesGizmo.h"

namespace itd
{
	std::shared_ptr<GrowingBlockAllocator<GameObject, 128>> GameObject::allocator_ = std::make_shared<GrowingBlockAllocator<GameObject, 128>>( );

	GameObject::GameObject( const std::string& name )
		: name_( name )
	{
		componentFilter_.reset( );
		transform_ = this->AddComponent<Transform>( );

#if defined(IL_DEBUG) || defined(IL_RELEASE)
		auto& gizmo = this->AddComponent<AxesGizmo>( );
#endif
	}

	GameObject::~GameObject( )
	{
		IL_CORE_INFO( "Destroyed {0}", name_ );
		transform_ = nullptr;

		for (DeleterMap::iterator it = deleterMap_.begin( ); it != deleterMap_.end( ); ++it)
		{
			it->second( );
		}

		deleterMap_.clear( );
		componentMap_.clear( );
		componentFilter_.reset( );
	}
}