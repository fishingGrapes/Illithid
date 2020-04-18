#pragma once

#include "illithid/game/Component.h"

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace itd
{
	enum class TransformationSpace : uint8_t
	{
		World, Local, Camera
	};

	class Transform : public Component<Transform, 256>
	{
		using ChildrenList = std::vector<dptr<Transform>>;

	public:
		glm::vec3 Position;
		glm::quat Orientation;
		glm::vec3 ScaleFactor;

		Transform( );
		~Transform( );

		void Translate( const glm::vec3& displacement );
		void Rotate( const glm::vec3& euler, TransformationSpace space = TransformationSpace::World );
		void Scale( const glm::vec3& factor );

		virtual void OnStart( ) override;
		virtual void OnUpdate( ) override;
		virtual void OnPreRender( ) override;
		virtual void OnRender( ) override;
		virtual void OnPostRender( ) override;

		glm::vec3 Forward( ) const;
		glm::vec3 Right( ) const;
		glm::vec3 Up( ) const;


		inline glm::mat4 TRS( ) const
		{
			return TRS_;
		}

		inline glm::mat4 InverseTRS( ) const
		{
			return inverseTRS_;
		}

		void SetParent( dptr<Transform> parent );
		void DetachChildren( );


		inline dptr<Transform> GetParent( ) const
		{
			return parent_;
		}

		inline bool IsRoot( ) const
		{
			return parent_ == nullptr;
		}

		inline size_t ChildCount( ) const
		{
			return children_.size( );
		}

		inline dptr<Transform> GetChild( size_t index ) const
		{
			return children_[ index ];
		}


	private:
		glm::mat4 TRS_;
		glm::mat4 inverseTRS_;

		dptr<Transform> parent_;
		ChildrenList children_;
	};

}