#include "ilpch.h"
#include "SceneGraph.h"

#include "GameObject.h"

namespace itd
{
	std::vector<GameObject*> SceneGraph::gameObjects_;

	void SceneGraph::Add( GameObject* object )
	{
		gameObjects_.push_back( object );
	}

	void SceneGraph::Remove( GameObject* object )
	{
		auto itr = std::find( gameObjects_.begin( ), gameObjects_.end( ), object );
		std::swap( itr, ( gameObjects_.end( ) - 1 ) );
		gameObjects_.pop_back( );
	}

	void SceneGraph::Update( )
	{
		for each (GameObject * object in gameObjects_)
		{
			object->Update( );
		}
	}

	void SceneGraph::PreRender( )
	{
		for each (GameObject * object in gameObjects_)
		{
			object->PreRender( );
		}
	}

	void SceneGraph::Render( )
	{
		for each (GameObject * object in gameObjects_)
		{
			object->Render( );
		}
	}

	void SceneGraph::PostRender( )
	{
		for each (GameObject * object in gameObjects_)
		{
			object->PostRender( );
		}
	}
}