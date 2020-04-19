#pragma once
#include "Scene.h"

namespace itd
{
	class SceneManager
	{
	public:
		inline static void LoadScene( std::shared_ptr<Scene> scene )
		{
			scene_ = scene;
		}

		static void UpdateTransforms( );
		static void UnloadCurrentScene( );

	private:
		static std::shared_ptr<Scene> scene_;
	};
}
