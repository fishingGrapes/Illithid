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
		ptr_ref<T> AddComponent( params... args )
		{
			if (componentFilter_[ T::ID ])
			{
				IL_CORE_WARN( "Trying to add Component_{0} that already exists in the current GameObject.", T::ID );
				return  nullptr;
			}

			ptr_ref<T> t = T::Instantiate( std::forward<params>( args )... );
			t->SetOwner( this );

			deleterMap_[ T::ID ] = [ this ] ( )
			{
				ptr_ref<T> t = ptr_ref<ComponentBase>::dyn_cast<T>( componentMap_.at( T::ID ) );
				T::Destroy( t );
			};

			componentMap_[ T::ID ] = ptr_ref<T>::dyn_cast<ComponentBase>( t );
			componentFilter_.set( T::ID, 1 );

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

			ptr_ref<T> t = ptr_ref<ComponentBase>::dyn_cast<T>( componentMap_.at( T::ID ) );
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

		inline ptr_ref<Transform> GetTransform( )
		{
			return transform_;
		}

		inline static ptr_ref<GameObject> Instantiate( const std::string& name )
		{
			ptr_ref<GameObject> dp = allocator_->instantiate( name );
			dp.get_data( )->dptr_address_ = dp.get_address( );
			return dp;
		}

		inline static void Destroy( ptr_ref<GameObject>& object )
		{
			allocator_->release( object.get_address( ) );
		}

		inline static void Destroy( GameObject* object )
		{
			allocator_->release( object->dptr_address_ );
			object->dptr_address_ = nullptr;
		}

	private:

		using ComponentMap = std::unordered_map<uint32_t, ptr_ref<ComponentBase>>;
		using DeleterMap = std::unordered_map<uint32_t, Deleter>;

		ComponentFilter componentFilter_;
		ComponentMap componentMap_;
		DeleterMap deleterMap_;

		std::string name_;
		ptr_ref<Transform> transform_;

		static std::shared_ptr<GrowingBlockAllocator<GameObject, 128>> allocator_;
		GameObject** dptr_address_;
	};
}
