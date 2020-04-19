#pragma once

#include <bitset>
#include <unordered_map>

#include "illithid/components/Transform.h"
#include "illithid/core/Log.h"


namespace itd
{
	//TODO: currently a max of 256 compoennts can be created
	using ComponentFilter = std::bitset<256>;
	class Transform;
	using Deleter = std::function<void( )>;

	class GameObject
	{

	public:
		GameObject( const std::string& name );
		~GameObject( );

		template<typename T, typename... params>
		dptr<T> AddComponent( params... args )
		{
			if (componentFilter_[ T::ID ])
			{
				IL_CORE_WARN( "Trying to add Component_{0} that already exists in the current GameObject.", T::ID );
				return  nullptr;
			}

			dptr<T> t = T::Instantiate( std::forward<params>( args )... );
			t->SetOwner( this );

			deleterMap_[ T::ID ] = [ this ] ( )
			{
				dptr<T> t = dptr<ComponentBase>::dyn_cast<T>( componentMap_.at( T::ID ) );
				T::Destroy( t );
			};

			componentMap_[ T::ID ] = dptr<T>::dyn_cast<ComponentBase>( t );
			componentFilter_.set( T::ID, 1 );

			t->OnStart( );
			return t;
		}

		template<typename T>
		dptr<T> GetComponent( )
		{
			if (!componentFilter_[ T::ID ])
			{
				IL_CORE_ERROR( "Trying to get Component_{0} that does not exist in the current GameObject", T::ID );
				return  nullptr;
			}

			dptr<T> t = dptr<ComponentBase>::dyn_cast<T>( componentMap_.at( T::ID ) );
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

		inline dptr<Transform> GetTransform( )
		{
			return transform_;
		}

		inline std::string& GetName( )
		{
			return name_;
		}

		inline static GameObject* Instantiate( const std::string& name )
		{
			return new GameObject( name );
		}

		inline static void Destroy( GameObject* object )
		{
			delete object;
		}

	private:

		using ComponentMap = std::unordered_map<uint32_t, dptr<ComponentBase>>;
		using DeleterMap = std::unordered_map<uint32_t, Deleter>;

		ComponentFilter componentFilter_;
		ComponentMap componentMap_;
		DeleterMap deleterMap_;

		std::string name_;
		dptr<Transform> transform_;

		void operator delete( void* p )
		{
			free( p );
		}
	};

}


