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
		friend class Application;

	public:
		static const uint32_t ID;
		virtual ~Component( )
		{

		}

		virtual void OnStart( ) = 0;
		virtual void OnUpdate( ) = 0;
		virtual void OnPreRender( ) = 0;
		virtual void OnRender( ) = 0;
		virtual void OnPostRender( ) = 0;

	protected:
		GameObject* gameObject_;

	private:
		friend class GameObject;
		inline void SetOwner( GameObject* object )
		{
			gameObject_ = object;
		}
	};

	template<typename T>
	const uint32_t Component<T>::ID( ComponentBase::RegisterComponent( ) );
}