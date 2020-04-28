#include "ilpch.h"
#include "Transform.h"

#include "illithid/core/Log.h"
#include "illithid/core/LogExtensions.h"

#include "glm/gtc/matrix_transform.hpp"
#include "illithid/game/GameObject.h"

namespace itd
{
	Transform::Transform( )
		: Position( glm::vec3( 0.0f ) ), Orientation( glm::quat( 1.0f, 0.0f, 0.0f, 0.0f ) ), ScaleFactor( glm::vec3( 1.0f ) ),
		TRS_( glm::mat4( 1.0f ) ), inverseTRS_( glm::mat4( 1.0f ) ), eulers_( glm::vec3( 0.0f ) ),
		parent_( nullptr )
	{
	}

	Transform::~Transform( )
	{
		parent_ = nullptr;
		children_.clear( );
	}


	void Transform::Translate( const glm::vec3& displacement )
	{
		Position += displacement;
	}

	void Transform::Rotate( const glm::vec3& euler, TransformationSpace space )
	{
		eulers_ = euler;

		//if (!IsRoot( ))
		//{
		//	eulers_ += parent_->eulers_;
		//}

		glm::quat& delta_x = glm::angleAxis( euler.x, glm::vec3( 1.0f, 0.0f, 0.0f ) );	//pitch
		glm::quat& delta_y = glm::angleAxis( euler.y, glm::vec3( 0.0f, 1.0f, 0.0f ) );	//yaw
		glm::quat& delta_z = glm::angleAxis( euler.z, glm::vec3( 0.0f, 0.0f, 1.0f ) );	//roll

		switch (space)
		{
			case TransformationSpace::World:
				{
					if (!IsRoot( ))
					{
						glm::quat& p_delta_x = glm::angleAxis( -parent_->eulers_.x, glm::vec3( 1.0f, 0.0f, 0.0f ) );
						glm::quat& p_delta_y = glm::angleAxis( -parent_->eulers_.y, glm::vec3( 0.0f, 1.0f, 0.0f ) );
						glm::quat& p_delta_z = glm::angleAxis( -parent_->eulers_.z, glm::vec3( 0.0f, 0.0f, 1.0f ) );

						Orientation = p_delta_x * Orientation * delta_x;
						Orientation = p_delta_y * Orientation * delta_y;
						Orientation = p_delta_z * Orientation * delta_z;
					}
					else
					{
						Orientation = Orientation * delta_x;
						Orientation = Orientation * delta_y;
						Orientation = Orientation * delta_z;
					}

					break;
				}

			case TransformationSpace::Local:
				Orientation = Orientation * delta_x;
				Orientation = Orientation * delta_y;
				Orientation = Orientation * delta_z;
				break;

			case TransformationSpace::Camera:
				Orientation = delta_y * Orientation;
				Orientation = Orientation * delta_x;
				Orientation = Orientation * delta_z;
				break;

		}

		Orientation = glm::normalize( Orientation );
	}

	void Transform::Scale( const glm::vec3& factor )
	{
		ScaleFactor *= factor;
	}

	glm::vec3 Transform::Forward( ) const
	{
		return Orientation * glm::vec3( 0.0f, 0.0f, -1.0f );
	}

	glm::vec3 Transform::Right( ) const
	{
		return Orientation * glm::vec3( 1.0f, 0.0f, 0.0f );
	}

	glm::vec3 Transform::Up( ) const
	{
		return Orientation * glm::vec3( 0.0f, 1.0f, 0.0f );
	}

	void Transform::SetParent( dptr<Transform> parent )
	{
		parent_ = parent;
		parent_->children_.push_back( gameObject->GetComponent<Transform>( ) );
	}

	void Transform::DetachChildren( )
	{
		for each (auto & child in children_)
		{
			//Use SiblingIndex to find the iterator quickly
			parent_->children_.erase( std::find( parent_->children_.begin( ), parent_->children_.end( ), child ) );
			child->SetParent( parent_ );
		}
	}


	void Transform::OnStart( )
	{
	}

	void Transform::OnUpdate( )
	{
		glm::mat4 translateMatrix = glm::mat4( 1.0f );
		translateMatrix = glm::translate( translateMatrix, Position );

		glm::mat4 rotateMatrix = glm::mat4_cast( Orientation );

		glm::mat4 scaleMatrix = glm::mat4( 1.0f );
		scaleMatrix = glm::scale( scaleMatrix, ScaleFactor );

		glm::mat4 transformation = IsRoot( ) ? glm::mat4( 1.0f ) : parent_->TRS( );
		TRS_ = transformation * ( translateMatrix * rotateMatrix * scaleMatrix );

		translateMatrix = glm::mat4( 1.0f );
		transformation = IsRoot( ) ? glm::mat4( 1.0f ) : parent_->InverseTRS( );
		inverseTRS_ = transformation * ( glm::scale( scaleMatrix, 1.0f / ScaleFactor ) * glm::transpose( rotateMatrix ) * glm::translate( translateMatrix, -1.0f * Position ) );


		for each (auto & child in children_)
		{
			child->OnUpdate( );
		}
	}

	void Transform::OnPreRender( )
	{
	}

	void Transform::OnRender( )
	{
	}

	void Transform::OnPostRender( )
	{
		eulers_ = glm::vec3( 0.0f );
	}

}