#pragma once

#include <bitset>
#include <unordered_map>

#include "illithid/components/Transform.h"
#include "illithid/core/Log.h"

namespace itd
{
	//TODO: currently a max of 256 compoennts can be created
	using ComponentFilter = std::bitset<256>;
	using Deleter = std::function<void( )>;
	class Transform;

	class GameObject
	{

	public:
		GameObject( );

		//TODO: Disable all components on Destruction
		~GameObject( );

		template<typename T, typename... params>
		ptr_ref<T> AddComponent( params... args )
		{
			if (componentFilter_[ T::ID ])
			{
				IL_CORE_WARN( "Trying to add Component_{0} that already exists in the current GameObject.", T::ID );
				return  nullptr;
			}

			ptr_ref<T> t = T::Instantiate( std::forward<params>( args )... );
			t->SetOwner( this );

			componentMap_[ T::ID ] = ptr_ref<T>::cast<ComponentBase>( t );
			componentFilter_.set( T::ID, 1 );
			deleterMap_[ T::ID ] = [ this ] ( )
			{
				ptr_ref<T> t = ptr_ref<ComponentBase>::cast<T>( componentMap_.at( T::ID ) );
				T::Destroy( t );
			};

			t->OnStart( );
			return t;
		}

		template<typename T>
		ptr_ref<T> GetComponent( )
		{
			if (!componentFilter_[ T::ID ])
			{
				IL_CORE_ERROR( "Trying to get Component_{0} that does not exist in the current GameObject", T::ID );
				return  nullptr;
			}

			ptr_ref<T> t = ptr_ref<ComponentBase>::cast<T>( componentMap_.at( T::ID ) );
			return t;
		}

		template<typename T>
		bool HasComponent( )
		{
			return componentFilter_[ T::ID ];
		}

		template<typename T>
		void RemoveComponent( )
		{
			if (!componentFilter_[ T::ID ])
			{
				IL_CORE_ERROR( "Trying to remove Component_{0} that does not exist in the current GameObject", T::ID );
				return;
			}

			deleterMap_[ T::ID ]( );

			componentMap_.erase( T::ID );
			deleterMap_.erase( T::ID );
			componentFilter_.set( T::ID, 0 );
		}

		inline ptr_ref<Transform> GetTransform( ) const
		{
			return transform_;
		}

	private:

		using ComponentMap = std::unordered_map<uint32_t, ptr_ref<ComponentBase>>;
		using DeleterMap = std::unordered_map<uint32_t, Deleter>;

		ComponentFilter componentFilter_;
		ComponentMap componentMap_;
		DeleterMap deleterMap_;
		ptr_ref<Transform> transform_;
	};
}
