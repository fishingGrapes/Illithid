#pragma once
#include <stdint.h>

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

	template<typename T>
	class Component : public ComponentBase
	{

	public:
		static const uint32_t ID;

		Component( )
			:Enabled( true )
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

		/*template<typename... params>
		static std::shared_ptr<T> Instantiate( params... args )
		{
			return allocator_.make_shared( std::forward<params>( args )... );
		}*/

	protected:
		GameObject* gameObject_;

	private:
		friend class GameObject;
		inline void SetOwner( GameObject* object )
		{
			gameObject_ = object;
		}

		//static GrowingAllocator<T> allocator_;
	};

	/*template<typename T>
	GrowingAllocator<T> Component<T>::allocator_;*/

	template<typename T>
	const uint32_t Component<T>::ID( ComponentBase::RegisterComponent( ) );
}