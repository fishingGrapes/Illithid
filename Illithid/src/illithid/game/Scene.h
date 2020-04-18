#pragma once
#include <vector>
#include "GameObject.h"

namespace itd
{
	class Scene
	{
		using GameObjects = std::vector<GameObject*>;

	public:
		inline void AddRootObject( GameObject* object )
		{
			roots_.push_back( object );
		}

		inline size_t RootCount( ) const
		{
			return roots_.size( );
		}

		inline GameObject* GetRootObject( size_t index ) const
		{
			return roots_[ index ];
		}

	private:
		GameObjects roots_;
	};
}