#include "ilpch.h"
#include "ComponentRegistry.h"

namespace itd
{
	using  ComponentFunction = std::function<void( )>;
	std::vector<ComponentFunction> ComponentRegistry::starts_;
	std::vector<ComponentFunction> ComponentRegistry::updates_;
	std::vector<ComponentFunction> ComponentRegistry::prerenders_;
	std::vector<ComponentFunction> ComponentRegistry::renders_;
	std::vector<ComponentFunction> ComponentRegistry::postrenders_;

	void ComponentRegistry::Start( )
	{
		for (size_t i = 0; i < starts_.size( ); i++)
		{
			starts_[ i ]( );
		}
	}

	void ComponentRegistry::Update( )
	{
		for (size_t i = 0; i < updates_.size( ); i++)
		{
			updates_[ i ]( );
		}
	}

	void ComponentRegistry::PreRender( )
	{
		for (size_t i = 0; i < prerenders_.size( ); i++)
		{
			prerenders_[ i ]( );
		}
	}

	void ComponentRegistry::Render( )
	{
		for (size_t i = 0; i < renders_.size( ); i++)
		{
			renders_[ i ]( );
		}
	}

	void ComponentRegistry::PostRender( )
	{
		for (size_t i = 0; i < postrenders_.size( ); i++)
		{
			postrenders_[ i ]( );
		}
	}

	void ComponentRegistry::Destroy( )
	{
		starts_.clear( );
		updates_.clear( );
		prerenders_.clear( );
		renders_.clear( );
		postrenders_.clear( );
	}
}