#pragma once
#include <memory>
#include <set>

#include "RendererConstants.h"
#include "StaticMesh.h"
#include "Material.h"
#include "Texture2D.h"
#include "LineSegment.h"
#include "illithid/components/MeshRenderer.h"

namespace itd
{
	class Window;
	class Graphics
	{
	public:
		static void Initialize( std::unique_ptr<Window>& window );

		static void Clear( int32_t bits );
		static void ClearColor( float_t r, float_t g, float_t b, float_t a );

		static void EnableCapabality( int32_t capability );
		static void DisableCapabality( int32_t capability );

		static void SetPolygonMode( PolygonFace face, PolygonMode mode );

		static void DrawMesh( StaticMesh& mesh, Material& material );
		static void DrawLineSegment( LineSegment& mesh, Material& material );

		static void BuildRenderGraph( );
		static void DrawRenderGraph( );

	private:
		using GLenum = uint32_t;
		using GLsizei = int32_t;
		using GLchar = char;


		static void GLErrorCallback( GLenum source, GLenum type, GLenum id, GLenum severity, GLsizei length,
									 const GLchar* message, const void* userParam );


		struct Comparator
		{
		public:
			inline bool operator()( MeshRenderer* lhs, MeshRenderer* rhs ) const
			{
				return  ( lhs->Material->ShaderQueue < rhs->Material->ShaderQueue );
			}
		};
		static std::multiset<MeshRenderer*, Comparator> renderGraph_;

	};
}