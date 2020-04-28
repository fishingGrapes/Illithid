#include "ilpch.h"
#include "GameObject.h"

#include "illithid/components/AxesGizmo.h"

namespace itd
{

	GameObject::GameObject( const std::string& name, bool is_static )
		: name_( name ), Static( is_static )
	{
		IL_CORE_INFO( "Created {0}", name_ );

		componentFilter_.reset( );
		transform_ = this->AddComponent<Transform>( );

#if defined(IL_DEBUG) || defined(IL_RELEASE)
		auto& gizmo = this->AddComponent<AxesGizmo>( );
#endif
	}

	GameObject::~GameObject( )
	{
		for (size_t i = 0; i < transform_->ChildCount( ); ++i)
		{
			GameObject::Destroy( transform_->GetChild( i )->gameObject );
		}

		for (DeleterMap::iterator it = deleterMap_.begin( ); it != deleterMap_.end( ); ++it)
		{
			it->second( );
		}

		deleterMap_.clear( );
		componentMap_.clear( );
		componentFilter_.reset( );

		transform_ = nullptr;
		IL_CORE_INFO( "Destroyed {0}", name_ );
	}
}