#pragma once

#include "Illithid.h"
#include "illithid/core/EntryPoint.h"


struct Data
{
public:
	int32_t Age;
	int32_t* Marks;

	Data( )
		:Age( glm::linearRand( 18, 50 ) )
	{
		Marks = new int32_t[ 5 ];
		IL_WARN( "Data is Constructed" );
	}

	~Data( )
	{
		delete[ ] Marks;
		IL_WARN( "Data is Destroyed" );
	}
};

struct Pack
{
public:

	Pack( int32_t v1, int16_t v2 )
		:value1( v1 ), value2( v2 )
	{
		IL_WARN( "Constructed Pack {0}", (void*) this );
	}

	~Pack( )
	{
		IL_WARN( "Destroyed Pack {0}", (void*) this );
	}

	int32_t value1;
	int16_t value2;
};

class TestApplication : public itd::Application
{

private:
	PoolAllocator<Pack> packAlloc_;

public:

	TestApplication( )
	{
	}

	~TestApplication( )
	{
	}

	// Inherited via Application
	virtual void Start( ) override
	{
		std::vector<std::shared_ptr<Pack>> packs_;

		{
			for (size_t i = 0; i < 8; i++)
			{
				packs_.push_back( packAlloc_.make_shared( static_cast<int32_t>( i + 1 ), static_cast<int16_t>( ( i + 1 ) * 10 ) ) );
			}
			IL_TRACE( "Size: {0}", packAlloc_.size( ) );

			packAlloc_.release( packs_[ 2 ] );
			packs_[ 2 ] = packAlloc_.make_shared( 1, 10 );

			IL_TRACE( "Size: {0}", packAlloc_.size( ) );
		}


	}

	virtual void Shutdown( ) override
	{
		IL_TRACE( "Shutdown" );
	}

	virtual void OnEvent( itd::Event& event ) override
	{
		Application::OnEvent( event );
	}



	// Inherited via Application
	virtual void Update( ) override
	{
	}


	// Inherited via Application
	virtual void PreRender( ) override
	{
	}

};

SET_ILLITHID_APPLICATION( TestApplication );
