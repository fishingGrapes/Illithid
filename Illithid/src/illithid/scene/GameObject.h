#pragma once

#include <bitset>
#include <vector>
#include <unordered_map>
#include <memory>

#include "illithid/components/Transform.h"
#include "SceneGraph.h"

#include "illithid/core/Log.h"

namespace itd
{
	//TODO: currently a max of 256 compoennts can be created
	using ComponentFilter = std::bitset<256>;
	class Transform;

	class GameObject
	{

	public:
		GameObject( );
		~GameObject( );

		template<typename T, typename... params>
		std::shared_ptr<T> AddComponent( params... args )
		{
			if (componentFilter_[ T::ID ])
			{
				IL_CORE_WARN( "Trying to add Component_{0} that already exists in the current GameObject.", T::ID );
				return nullptr;
			}

			std::shared_ptr<T> t = std::make_shared<T>( std::forward<params>( args )... );
			t->SetOwner( this );

			components_.push_back( t );
			componentMap_[ T::ID ] = t;
			componentFilter_.set( T::ID, 1 );

			t->OnStart( );
			return t;
		}

		template<typename T>
		std::shared_ptr<T> GetComponent( )
		{
			if (!componentFilter_[ T::ID ])
			{
				IL_CORE_ERROR( "Trying to get Component_{0} that does not exist in the current GameObject", T::ID );
				return  nullptr;
			}

			std::shared_ptr<T> t = std::dynamic_pointer_cast<T>( componentMap_.at( T::ID ) );
			return t;
		}

		template<typename T>
		void RemoveComponent( )
		{
			if (!componentFilter_[ T::ID ])
			{
				IL_CORE_ERROR( "Trying to remove Component_{0} that does not exist in the current GameObject", T::ID );
				return;
			}

			auto itr = std::find_if( components_.begin( ), components_.end( ), [ this ] ( std::shared_ptr<T> const& ptr )
			{
				return ptr.get( ) == ( componentMap_.at( T::ID ) ).get( );
			} );

			std::swap( itr, ( components_.end( ) - 1 ) );
			components_.pop_back( );

			componentMap_.erase( T::ID );
			componentFilter_.set( T::ID, 0 );
		}

		inline std::shared_ptr<Transform> GetTransform( ) const
		{
			return transform_;
		}

	private:
		std::vector<std::shared_ptr<ComponentBase>> components_;
		ComponentFilter componentFilter_;
		std::unordered_map<uint32_t, std::shared_ptr<ComponentBase>> componentMap_;

		std::shared_ptr<Transform> transform_;

		friend class SceneGraph;

		void Update( );
		void PreRender( );
		void Render( );
		void PostRender( );
	};
}