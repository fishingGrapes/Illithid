#pragma once
#include <memory>
#include <functional>

namespace itd
{

	class ComponentRegistry
	{
		using  ComponentFunction = std::function<void( )>;

	public:
		template <typename T>
		static void Register( bool start, bool update, bool prerender, bool render, bool postrender )
		{
			auto allocator = T::get_allocator( );

			if (start)
			{
				starts_.push_back( [ allocator ] ( )
				{
					auto& alloc_ref = *( allocator.get( ) );
					for (size_t i = 0; i < alloc_ref.size( ); i++)
					{
						auto component = alloc_ref[ i ];
						component->OnStart( );
					}
				} );
			}

			if (update)
			{
				updates_.push_back( [ allocator ] ( )
				{
					auto& alloc_ref = *( allocator.get( ) );
					for (size_t i = 0; i < alloc_ref.size( ); i++)
					{
						auto component = alloc_ref[ i ];
						component->OnUpdate( );
					}
				} );
			}

			if (prerender)
			{
				prerenders_.push_back( [ allocator ] ( )
				{
					auto& alloc_ref = *( allocator.get( ) );
					for (size_t i = 0; i < alloc_ref.size( ); i++)
					{
						auto component = alloc_ref[ i ];
						component->OnPreRender( );
					}
				} );
			}

			if (render)
			{
				renders_.push_back( [ allocator ] ( )
				{
					auto& alloc_ref = *( allocator.get( ) );
					for (size_t i = 0; i < alloc_ref.size( ); i++)
					{
						auto component = alloc_ref[ i ];
						component->OnRender( );
					}
				} );
			}

			if (postrender)
			{
				postrenders_.push_back( [ allocator ] ( )
				{
					auto& alloc_ref = *( allocator.get( ) );
					for (size_t i = 0; i < alloc_ref.size( ); i++)
					{
						auto component = alloc_ref[ i ];
						component->OnPostRender( );
					}
				} );
			}


		}

		static void Start( );
		static void Update( );
		static void PreRender( );
		static void Render( );
		static void PostRender( );


		static void Destroy( );

	private:
		static std::vector<ComponentFunction> starts_;
		static std::vector<ComponentFunction> updates_;
		static std::vector<ComponentFunction> prerenders_;
		static std::vector<ComponentFunction> renders_;
		static std::vector<ComponentFunction> postrenders_;

	};
}

