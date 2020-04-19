#include "ilpch.h"
#include "SceneManager.h"

namespace itd
{
	std::shared_ptr<Scene> SceneManager::scene_ = nullptr;

	void SceneManager::UpdateTransforms( )
	{
		for (size_t i = 0; i < scene_->RootCount( ); i++)
		{
			dptr<Transform> transform = scene_->GetRootObject( i )->GetTransform( );
			transform->OnUpdate( );
		}
	}

	void SceneManager::UnloadCurrentScene( )
	{
		for (size_t i = 0; i < scene_->RootCount( ); i++)
		{
			GameObject::Destroy( scene_->GetRootObject( i ) );
		}
	}
}