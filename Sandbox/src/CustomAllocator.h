#pragma once

#include "Illithid.h"
#include "illithid/core/EntryPoint.h"


struct Data
{
public:
	int32_t* Marks;
	int32_t id;

	Data( int32_t id_ ) :id( id_ )
	{
		Marks = new int32_t[ 5 ];
		IL_WARN( "Data_{0} is Constructed", id );
	}

	~Data( )
	{
		delete[ ] Marks;
		IL_WARN( "Data_{0} is Destroyed", id );
	}
};

struct Pack
{
public:
	Data* data;
	Pack( int32_t v1, int16_t v2 )
		:value1( v1 ), value2( v2 )
	{
		IL_WARN( "Constructed Pack_{1} at {0}", (void*) this, value1 );
		data = new Data( v1 );
	}

	~Pack( )
	{
		delete data;
		IL_WARN( "Destroyed Pack_{1} at {0}", (void*) this, value1 );
	}

	int32_t value1;
	int16_t value2;
};

class TestApplication : public itd::Application
{

private:

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
		GrowingBlockAllocator<Pack, 3> packAlloc_;


		std::vector< ptr_ref<Pack>> packs_;
		{
			for (size_t i = 0; i < 7; i++)
			{
				packs_.push_back( packAlloc_.instantiate( static_cast<int32_t>( 1 + i ), static_cast<int16_t>( ( 1 + i ) * 10 ) ) );
			}
		}

		packAlloc_.release( packs_[ 1 ] );
		packAlloc_.release( packs_[ 4 ] );
		packs_[ 4 ] = packAlloc_.instantiate( 8, 80 );
		for (size_t i = 0; i < packAlloc_.size( ); i++)
		{
			IL_TRACE( packAlloc_[ i ]->value1 );
		}

		/*	packAlloc_.release( packs_[ 3 ] );
			packs_[ 3 ] = packAlloc_.instantiate( 111, 111 );
			IL_TRACE( "{0}", packs_[ 5 ]->value1 );*/

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
