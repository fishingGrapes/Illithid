#pragma once
#include <vector>

namespace itd
{
	class GameObject;

	class SceneGraph
	{
	public:
		static void Add( GameObject* object );
		static void Remove( GameObject* object );

		static void Update( );
		static void PreRender( );
		static void Render( );
		static void PostRender( );

	private:
		static std::vector<GameObject*> gameObjects_;
	};
}
