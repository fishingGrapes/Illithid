#pragma once
#include <vector>
#include "illithid/utils/ptr.h"
#include "GameObject.h"

namespace itd
{
	class Scene
	{
		using GameObjects = std::vector<ptr<GameObject>>;

	public:
		inline void AddRootObject( ptr<GameObject> object )
		{
			roots_.push_back( object );
		}

		inline size_t RootCount( ) const
		{
			return roots_.size( );
		}

		inline ptr<GameObject> GetRootObject( size_t index ) const
		{
			return roots_[ index ];
		}

	private:
		GameObjects roots_;
	};
}