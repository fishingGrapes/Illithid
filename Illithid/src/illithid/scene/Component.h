#pragma once
#include <stdint.h>
#include <memory>
#include "illithid/memory/GrowingBlockAllocator.h"
#include "illithid/core/Log.h"

namespace itd
{
	static size_t allocations;
	class GameObject;

	class ComponentBase
	{
	public:
		ComponentBase( )
		{
			++allocations;
			IL_CORE_WARN( "Allocated: {0} components in memory", allocations );
		}

		virtual ~ComponentBase( )
		{
			--allocations;
			IL_CORE_WARN( "Deallocated: {0} components in memory", allocations );
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
			:Enabled( true ), gameObject_( nullptr )
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
		static ptr_ref<T> Instantiate( params... args )
		{
			return allocator_->instantiate( std::forward<params>( args )... );
		}

		static void Destroy( ptr_ref<T>& component )
		{
			allocator_->release( component );
		}

		inline static std::shared_ptr<GrowingBlockAllocator<T, BLOCK_SIZE>> get_allocator( )
		{
			return allocator_;
		}

	protected:
		GameObject* gameObject_;

	private:
		friend class GameObject;
		inline void SetOwner( GameObject* object )
		{
			gameObject_ = object;
		}

		static std::shared_ptr<GrowingBlockAllocator<T, BLOCK_SIZE>> allocator_;
	};

	template <typename T, size_t BLOCK_SIZE>
	std::shared_ptr<GrowingBlockAllocator<T, BLOCK_SIZE>> Component<T, BLOCK_SIZE>::allocator_ = std::make_shared<GrowingBlockAllocator<T, BLOCK_SIZE>>( );

	template<typename T, size_t BLOCK_SIZE>
	const uint32_t Component<T, BLOCK_SIZE>::ID( ComponentBase::RegisterComponent( ) );
}