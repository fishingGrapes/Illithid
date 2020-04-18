#pragma once
#include <stdint.h>
#include <memory>
#include "illithid/memory/GrowingBlockAllocator.h"
#include "illithid/utils/dptr.h"
#include "illithid/core/Log.h"

namespace itd
{
	class GameObject;

	class ComponentBase
	{
	public:

		virtual ~ComponentBase( )
		{

		}

		virtual void OnStart( ) = 0;
		virtual void OnUpdate( ) = 0;
		virtual void OnPreRender( ) = 0;
		virtual void OnRender( ) = 0;
		virtual void OnPostRender( ) = 0;

		static uint32_t RegisterComponent( )
		{
			static uint32_t componentID = 0;
			++componentID;
			return componentID;
		}

	};


	template<typename T, size_t BLOCK_SIZE>
	class Component : public ComponentBase
	{

	public:
		static const uint32_t ID;

		Component( )
			:Enabled( true ), gameObject( nullptr )
		{

		}

		virtual ~Component( )
		{

		}

		virtual void OnStart( ) = 0;
		virtual void OnUpdate( ) = 0;
		virtual void OnPreRender( ) = 0;
		virtual void OnRender( ) = 0;
		virtual void OnPostRender( ) = 0;

		bool Enabled;

		template<typename... params>
		static dptr<T> Instantiate( params... args )
		{
			return dptr<T>( allocator_->instantiate( std::forward<params>( args )... ) );
		}

		static void Destroy( dptr<T>& component )
		{
			allocator_->release( component.get_address( ) );
		}

		inline static std::shared_ptr<GrowingBlockAllocator<T, BLOCK_SIZE>> get_allocator( )
		{
			return allocator_;
		}

		GameObject* const gameObject;

	protected:
		void operator delete( void* p )
		{
			free( p );
		}

	private:
		friend class GameObject;
		inline void SetOwner( GameObject* object )
		{
			const_cast<GameObject*>( gameObject ) = object;
		}

		friend class GrowingBlockAllocator<T, BLOCK_SIZE>;
		static std::shared_ptr<GrowingBlockAllocator<T, BLOCK_SIZE>> allocator_;


	};

	template <typename T, size_t BLOCK_SIZE>
	std::shared_ptr<GrowingBlockAllocator<T, BLOCK_SIZE>> Component<T, BLOCK_SIZE>::allocator_ = std::make_shared<GrowingBlockAllocator<T, BLOCK_SIZE>>( );

	template<typename T, size_t BLOCK_SIZE>
	const uint32_t Component<T, BLOCK_SIZE>::ID( ComponentBase::RegisterComponent( ) );
}